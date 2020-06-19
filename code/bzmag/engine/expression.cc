#include "expression.h"
#include "expressionserver.h"
#include "core/simplepropertybinder.h"

#include <algorithm>

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(Expression, Object);


mu::Parser Expression::m_Parser;

//----------------------------------------------------------------------------
mu::string_type Expression::mustring(String const& str)
{
    mu::string_type mustr;
    mustr.assign(str.begin(), str.end());

    return mustr;
}

//----------------------------------------------------------------------------
Expression::Expression(void) :
    value_(0), hidden_(true), userdefined_(true), expression_("0")
{
    ExpressionServer* server = ExpressionServer::instance();
    server->addExpression(this);
}

//----------------------------------------------------------------------------
Expression::~Expression(void)
{

}

//----------------------------------------------------------------------------
bool Expression::setKey(const String& sKey)
{
    ExpressionServer* server = ExpressionServer::instance();

    // �̹� ��ϵ� Ű��� ����
    Expression* expr = server->findExpression(sKey);
    if(0 != expr) return false;

    // �̹� Ű�� �����Ǿ� �ִٸ� ����
    if (!key_.empty()) return false;

    // Ű�� ��ȿ�� üũ
    if (!checkValidKey(sKey))
        return false;

    // Ű�� ������
    key_ = sKey;

    // �ļ��� ������ ����� (ǥ������ Ű ���)
    mu::string_type ssKey = mustring(sKey);
    m_Parser.DefineVar(ssKey, &value_);

    return true;
}

//----------------------------------------------------------------------------
bool Expression::setExpression(const String& sExpr)
{
    ExpressionServer* server = ExpressionServer::instance();

    // �̹� ������ ���� ���ٸ� ���̻� ������ �ʿ� ����
    if (expression_ == sExpr) return true;

    // ������Ʈ�ϰ��� �ϴ� ǥ���Ŀ� ������ ������ �Ǵ�
    String sKey = getKey();
    if (!checkConsistancy(sKey, sExpr)) return false;

    // ���� ���� ���۷��� �� ī���� (���� �����ߴ� �����鿡 ���Ͽ�...)
    RefExprIter it;
    for (it = firstUsedItem(); it != lastUsedItem(); ++it)
    {
        Expression* UsedExpr = *it;
        UsedExpr->deleteLinkedExpr(this);
    }
    usedItems_.clear();

    // ���ο� ���� ���۷��� ī����
    mu::string_type ssVal = mustring(sExpr);
    m_Parser.SetExpr(ssVal);
    mu::varmap_type variables = m_Parser.GetUsedVar();
    mu::varmap_type::const_iterator item;
    for (item = variables.begin(); item != variables.end(); ++item)
    {
        mu::string_type sKey = item->first;

        // �־��� key�� ���� ���ã��
        String key(sKey.c_str());
        Expression* expr = server->findExpression(key);

        if (expr)
        {
            // ���� �����ϰ� �ִ� ���� ������Ʈ
            expr->addLinkedExpr(this);

            // ���� �����ϰ� �ִ� ���� ������Ʈ
            addUsedExpr(expr);
        }
    }

    // ǥ���� ����
    expression_ = sExpr;

    // ���� ���
    return update();
}

//----------------------------------------------------------------------------
void Expression::addLinkedExpr(Expression* pExpr)
{
    ExprIter it
        = std::find(linkedItems_.begin(), linkedItems_.end(), pExpr->getKey());

    if (it == linkedItems_.end())
        linkedItems_.push_back(pExpr);
}

//----------------------------------------------------------------------------
void Expression::deleteLinkedExpr(Expression* pExpr)
{
    ExprIter it
        = std::find(linkedItems_.begin(), linkedItems_.end(), pExpr->getKey());

    if (it != linkedItems_.end())
        linkedItems_.erase(it);
}

//----------------------------------------------------------------------------
void Expression::addUsedExpr(Expression* pExpr)
{
    RefExprIter it
        = std::find(usedItems_.begin(), usedItems_.end(), pExpr->getKey());

    if (it != usedItems_.end())
        usedItems_.push_back(pExpr);
}

//----------------------------------------------------------------------------
void Expression::deleteUsedExpr(Expression* pExpr)
{
    RefExprIter it
        = std::find(usedItems_.begin(), usedItems_.end(), pExpr->getKey());

    if (it != usedItems_.end())
        usedItems_.erase(it);
}

//----------------------------------------------------------------------------
bool Expression::checkConsistancy(const String& sKey, const String& sVal)
{
    ExpressionServer* server = ExpressionServer::instance();

    // �ļ��� ǥ������ �����ϰ�,
    // ǥ���Ŀ� ���� �������� �̾Ƴ���
    mu::string_type ssKey = mustring(sKey);
    mu::string_type ssVal = mustring(sVal);

    // ǥ������ �Ľ��ؼ� �������� �������� �Ǵ�
    m_Parser.SetExpr(ssVal);
    mu::varmap_type variables;
    try {
        variables = m_Parser.GetUsedVar();
    }
    catch (...) {
        return false;
    }

    // ǥ���Ŀ� ���� Ű�� ���� �־��� ǥ������ Ű�� �����ϸ� ��ȯ�����̴�
    mu::varmap_type::const_iterator item;
    for (item = variables.begin(); item != variables.end(); ++item)
    {
        std::wstring var_name = item->first;
        if (var_name == ssKey)
            return false;

        // ǥ���Ŀ� ���� Ű�� ǥ���� �� �ϳ��� ��ϵǾ� ���� �ʰų�, 
        // ���� �־��� Ű�� �����ϸ� ��ȯ������
        String nextKey(var_name.c_str());

        Expression* pRefExpr = server->findExpression(nextKey);

        if (pRefExpr != 0) {
            if (!checkConsistancy(sKey, pRefExpr->getExpression())) {
                return false;
            }
        }

        // �־��� ǥ���Ŀ� ���� ������ ��ϵ� ������ �ƴ϶�� �߸��� ���̴�
        else
            return false;
    }

    return true;
}

//----------------------------------------------------------------------------
bool Expression::checkValidKey(const String& sKey)
{
    // ǥ������ �Ľ��ؼ� �������� �������� �Ǵ�
    mu::string_type key = mustring(sKey);
    m_Parser.SetExpr(key);
    mu::varmap_type variables;
    try {
        variables = m_Parser.GetUsedVar();
    }
    catch (...) {
        return false;
    }

    if (variables.size() != 1) return false;
    return true;
}

//----------------------------------------------------------------------------
float64 Expression::eval()
{
    mu::string_type sKey = mustring(key_);
    m_Parser.SetExpr(sKey);

    float64 val = m_Parser.Eval();
    assert(val == value_);
    return value_;

    // return m_Parser.Eval();
}


//----------------------------------------------------------------------------
bool Expression::update()
{
    String equation("%s=%s", key_.c_str(), expression_.c_str());
    mu::string_type expr = mustring(equation);
    m_Parser.SetExpr(expr);
    m_Parser.Eval();

    // ���� �����ϴ� ���ĵ��� ����
    ExprIter it;
    for (it = firstLinkedItem(); it != lastLinkedItem(); ++it)
    {
        Expression* pCitingExpr = *it;
        pCitingExpr->update();
    }
    return true;
}


//----------------------------------------------------------------------------
void Expression::bindProperty()
{
    BIND_PROPERTY(const String&, Key, 0, &getKey);
    BIND_PROPERTY(const String&, Expression, 0, &getExpression);
    BIND_PROPERTY(const String&, Comment, &setComment, &getComment);
    BIND_PROPERTY(bool, IsHidden, &setHidden, &isHidden);
    BIND_PROPERTY(bool, IsUserDefined, &setUserDefined, &isUserDefined);
//    BIND_PROPERTY(float64, value, 0, &eval);
}
