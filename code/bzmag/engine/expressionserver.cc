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
    // 해당 표현식을 저장소에서 삭제

    // 모든 Object는 Autorelease Pool 에 등록되어 있기 때문에
    // RefCount의 기본값은 1이다.
    // RefCount의 값이 1보다 크다면 해당 Object는 
    // Ref<>에 의해 다른 Object에서 참조하고 있다고 판단할 수 있다
    // 이경우 서버에서 삭제하면 참조 오류를 발생시키므로  
    // false 리턴
    if (expr->getRef() > 1) {
        return false;
    }

    else
    {
        // 서버에서 삭제
        m_Expressions.erase(expr->getKey());

        // Auto release pool 에서 삭제 --> 메모리 헤제
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