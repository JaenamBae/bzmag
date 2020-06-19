#pragma once

/*
Description : Expression Server
Last Update : 2017.10.31
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "Expression.h"
#include "core/node.h"
#include "core/singleton.h"
#include "core/singleton3.h"
#include <map>


namespace bzmag
{
namespace engine
{
    class ExpressionServer : public Node, public Singleton3<ExpressionServer>
    {
    public:
        typedef std::map<String, Expression*> Expressions;
        typedef std::map<String, Expression*>::iterator ExprIter;
        typedef std::map<String, Expression*>::const_iterator const_ExprIter;

        ExpressionServer(void);
        virtual ~ExpressionServer(void);
        DECLARE_CLASS(ExpressionServer, Node);

        ExprIter firstExpression() { return m_Expressions.begin(); };
        ExprIter lastExpression() { return m_Expressions.end(); };
        const_ExprIter firstExpression() const { return m_Expressions.begin(); };
        const_ExprIter lastExpression() const  { return m_Expressions.end(); };

        bool addExpression(Expression* expr);
        bool removeExpression(Expression* expr);

        Expression* findExpression(const String& key);

    private:
        Expressions m_Expressions;
    };

}
}