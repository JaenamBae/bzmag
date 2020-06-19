#pragma once

/*
Description : Coordinate System Nodes
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geometric_entity.h"
#include "core/node.h"
#include <list>


namespace bzmag
{
namespace engine
{
    class Expression;
    class CSNode : public Node
    {
    public:
        typedef std::list<Node*> CitingNodes;
        typedef CitingNodes::iterator CitingIter;

    public:
        CSNode();
        virtual ~CSNode();
        DECLARE_CLASS(CSNode, Node);

        bool setParameters(const String& origin, const String& angle);
        void setOrigin(const String& origin);
        void setAngle(const String& angle);

        const String& getOrigin() const;
        const String& getAngle() const;

        float64 getGlobalOriginX() const;  // �۷ι� ��ǥ�� ����
        float64 getGlobalOriginY() const;  // �۷ι� ��ǥ�� ����
        float64 getGlobalAngle() const;    // �۷ι� ��ǥ�� ����

        Transformation& transformation();

        void insertReferenceNode(Node* node);
        void removeReferenceNode(Node* node);

    public:
        virtual bool update();
        virtual void onAttachTo(Node* parent);
        virtual void onDetachFrom(Node* parent);


    public:
        static void bindMethod();
        static void bindProperty();


    private:
        Ref<Expression> ox_;
        Ref<Expression> oy_;
        Ref<Expression> angle_;

        String sorigin_;
        String sangle_;

        Transformation transform_;

        // CS������ ����Ǹ� CS�� �����ϰ� �ִ� Node���� ������Ʈ ���Ѿ� �ϱ� ������
        // CS�� �����ϰ� �ִ� ����Ʈ�� �����ؾ� ��
        CitingNodes citingNodes_;
    };

#include "csnode.inl"

    }
}