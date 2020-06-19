#include "ExpressionServer.h"
#include "core/string.h"
#include "core/autoreleasepool.h"

using namespace bzmag;
using namespace bzmag::engine;


IMPLEMENT_CLASS(ExpressionServer, Node);

//----------------------------------------------------------------------------
ExpressionServer::ExpressionServer(void)
{

}

//----------------------------------------------------------------------------
ExpressionServer::~ExpressionServer(void)
{

}

//----------------------------------------------------------------------------
bool ExpressionServer::addExpression(Expression* expr)
{
    if (findExpression(expr->getKey()))
        return false;

    m_Expressions[expr->getKey()] = expr;
    return true;
}

//----------------------------------------------------------------------------
bool ExpressionServer::removeExpression(Expression* expr)
{
    // �ش� ǥ������ ����ҿ��� ����

    // ��� Object�� Autorelease Pool �� ��ϵǾ� �ֱ� ������
    // RefCount�� �⺻���� 1�̴�.
    // RefCount�� ���� 1���� ũ�ٸ� �ش� Object�� 
    // Ref<>�� ���� �ٸ� Object���� �����ϰ� �ִٰ� �Ǵ��� �� �ִ�
    // �̰�� �������� �����ϸ� ���� ������ �߻���Ű�Ƿ�  
    // false ����
    if (expr->getRef() > 1) {
        return false;
    }

    else
    {
        // �������� ����
        m_Expressions.erase(expr->getKey());

        // Auto release pool ���� ���� --> �޸� ����
        AutoReleasePool::instance()->remove(expr);

        return true;
    }
}

//----------------------------------------------------------------------------
Expression* ExpressionServer::findExpression(const String& key)
{
    ExprIter it = m_Expressions.find(key);
    if (it != m_Expressions.end())
        return it->second;

    return 0;
}