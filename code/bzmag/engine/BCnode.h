#pragma once

/*
Description : Boundary condition Node
Last Update : 2020.07.22
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/node.h"

namespace bzmag
{
namespace engine
{
	class Expression;
    class BCNode : public Node
    {
	public:
		enum BCTYPE {
			UNDEFINED = -1,	// 정의되지 않음
			NEUMANN,		// 자연경계(Natural BC)
			DIRICHLET,		// 고정경계(Fixed BC)
			PERIODIC		// 주기경계(Periodic BC)
		};

    public:
		BCNode();
        virtual ~BCNode();
        DECLARE_CLASS(BCNode, Node);

    public:
		// 경계조건 설정
		void setBCType(const BCTYPE type);
		BCTYPE getBCType() const;
		virtual bool setBCValue(const String& value);

    public:
        virtual bool update();
        virtual void onAttachTo(Node* parent);
        virtual void onDetachFrom(Node* parent);

    public:
        static void bindMethod();
        static void bindProperty();
    
    protected:
		BCTYPE type_;
		Ref<Expression> value_;
    };

#include "BCnode.inl"

}
}