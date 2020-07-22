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
			UNDEFINED = -1,	// ���ǵ��� ����
			NEUMANN,		// �ڿ����(Natural BC)
			DIRICHLET,		// �������(Fixed BC)
			PERIODIC		// �ֱ���(Periodic BC)
		};

    public:
		BCNode();
        virtual ~BCNode();
        DECLARE_CLASS(BCNode, Node);

    public:
		// ������� ����
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