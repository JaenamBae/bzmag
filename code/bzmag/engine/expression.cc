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

    // 이미 등록된 키라면 실패
    Expression* expr = server->findExpression(sKey);
    if(0 != expr) return false;

    // 이미 키가 설정되어 있다면 실패
    if (!key_.empty()) return false;

    // 키의 유효성 체크
    if (!checkValidKey(sKey))
        return false;

    // 키를 설정함
    key_ = sKey;

    // 파서에 변수를 등록함 (표현식의 키 등록)
    mu::string_type ssKey = mustring(sKey);
    m_Parser.DefineVar(ssKey, &value_);

    return true;
}

//----------------------------------------------------------------------------
bool Expression::setExpression(const String& sExpr)
{
    ExpressionServer* server = ExpressionServer::instance();

    // 이미 설정된 값과 같다면 더이상 진행할 필요 없음
    if (expression_ == sExpr) return true;

    // 업데이트하고자 하는 표현식에 문제가 없는지 판단
    String sKey = getKey();
    if (!checkConsistancy(sKey, sExpr)) return false;

    // 기존 식의 레퍼런스 디 카운팅 (내가 참조했던 변수들에 대하여...)
    RefExprIter it;
    for (it = firstUsedItem(); it != lastUsedItem(); ++it)
    {
        Expression* UsedExpr = *it;
        UsedExpr->deleteLinkedExpr(this);
    }
    usedItems_.clear();

    // 새로운 식의 레퍼런스 카운팅
    mu::string_type ssVal = mustring(sExpr);
    m_Parser.SetExpr(ssVal);
    mu::varmap_type variables = m_Parser.GetUsedVar();
    mu::varmap_type::const_iterator item;
    for (item = variables.begin(); item != variables.end(); ++item)
    {
        mu::string_type sKey = item->first;

        // 주어진 key에 대한 노드찾기
        String key(sKey.c_str());
        Expression* expr = server->findExpression(key);

        if (expr)
        {
            // 나를 참조하고 있는 수식 업데이트
            expr->addLinkedExpr(this);

            // 내가 참조하고 있는 수식 업데이트
            addUsedExpr(expr);
        }
    }

    // 표현식 설정
    expression_ = sExpr;

    // 수식 계산
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

    // 파서에 표현식을 설정하고,
    // 표현식에 사용된 변수들을 뽑아낸다
    mu::string_type ssKey = mustring(sKey);
    mu::string_type ssVal = mustring(sVal);

    // 표현식을 파싱해서 정상적인 수식인지 판단
    m_Parser.SetExpr(ssVal);
    mu::varmap_type variables;
    try {
        variables = m_Parser.GetUsedVar();
    }
    catch (...) {
        return false;
    }

    // 표현식에 사용된 키가 현재 주어진 표현식의 키와 동일하면 순환참조이다
    mu::varmap_type::const_iterator item;
    for (item = variables.begin(); item != variables.end(); ++item)
    {
        std::wstring var_name = item->first;
        if (var_name == ssKey)
            return false;

        // 표현식에 사용된 키의 표현식 중 하나라도 등록되어 있지 않거나, 
        // 현재 주어진 키와 동일하면 순환참조다
        String nextKey(var_name.c_str());

        Expression* pRefExpr = server->findExpression(nextKey);

        if (pRefExpr != 0) {
            if (!checkConsistancy(sKey, pRefExpr->getExpression())) {
                return false;
            }
        }

        // 주어진 표현식에 사용된 변수가 등록된 변수가 아니라면 잘못된 식이다
        else
            return false;
    }

    return true;
}

//----------------------------------------------------------------------------
bool Expression::checkValidKey(const String& sKey)
{
    // 표현식을 파싱해서 정상적인 수식인지 판단
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

    // 나를 참조하는 수식들의 재평가
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
