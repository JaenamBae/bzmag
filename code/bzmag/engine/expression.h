#pragma once

/*
Description : Expression Node
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/object.h"
#include "core/ref.h"
#include "muParser/muParser.h"
#include <list>


namespace bzmag
{
namespace engine
{
    class Expression : public Object
    {
    public:
        typedef Ref<Expression> RefExpression;
        typedef std::list<RefExpression> RefExpressions;
        typedef std::list<Expression*> Expressions;
        typedef Expressions::iterator ExprIter;
        typedef RefExpressions::iterator RefExprIter;
        friend bool operator == (const RefExpression& s1, const String& s2);
        friend bool operator == (const Expression* s1, const String& s2);

    public:
        Expression(void);
        virtual ~Expression(void);
        bool operator==(const String& key)
        {
            return key_ == key;
        };
        DECLARE_CLASS(Expression, Object);

    public:
        bool setKey(const String& sKey);
        bool setExpression(const String& sExpr);
        void setComment(const String& sComment);
        void setHidden(bool bHidden);
        void setUserDefined(bool userdefned);
        const String& getExpression() const;
        const String& getKey() const;
        const String& getComment() const;
        bool isHidden() const;
        bool isUserDefined() const;
        float64 eval();
        bool update();

    protected:
        void addLinkedExpr(Expression* pExpr);
        void deleteLinkedExpr(Expression* pExpr);
        uint64 getLinkCount() const;
        ExprIter firstLinkedItem();
        ExprIter lastLinkedItem();
        void addUsedExpr(Expression* pExpr);
        void deleteUsedExpr(Expression* pExpr);
        RefExprIter firstUsedItem();
        RefExprIter lastUsedItem();

        bool checkConsistancy(const String& sKey, const String& sVal);
        bool checkValidKey(const String& sKey);
        static mu::string_type mustring(String const& str);

    public:
        static void bindMethod();
        static void bindProperty();
        

    private:
        String expression_;
        String key_;
        String comment_;
        mu::value_type value_;

        // Geometry�� ���� �����Ǵ� ��������...?
        bool hidden_;

        // ����� ���� ǥ��������? --> ���ø����̼ǿ��� ������ ����� �����Ҷ� �����Ҷ��...
        bool userdefined_;

        // ���� �����ϰ� �ִ� ���ĵ�
        Expressions linkedItems_;

        // ���� �����ϰ� �ִ� ���ĵ�
        RefExpressions usedItems_;

    private:
        static mu::Parser m_Parser;
    };

#include "expression.inl"
}
}
