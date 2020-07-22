#pragma once

/*
Description : Head Node for Handling a Geometry Node
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
#include "core/color.h"

namespace bzmag
{
namespace engine
{
	class BCNode;
	class MaterialNode;
    class GeomHeadNode : public GeomBaseNode
    {
    public:
        GeomHeadNode();
        virtual ~GeomHeadNode();
        DECLARE_CLASS(GeomHeadNode, GeomBaseNode);

		// Curves �� �ε���, �������
		typedef std::map<uint64, Ref<BCNode>> BCs;	// Boundary Conditions

    public:
        // �÷�����
        void setColor(const Color& color);
        const Color& getColor() const;

        // Visualization�� ���� ���� ������?
        void setHideStatus(bool hide);
        bool isHide() const;

        // Boolean ���꿡 ���� �����Ǵ� �ʴ� �������?
        void setStandAlone(bool standalone);
        bool isStandAlone() const;

        // �ؼ��� ���� �� �������?
        void setModelNode(bool model);
        bool isModelNode() const;

        // Head��尡 �����ϴ� ���� ���� �ڽ� ���
        void setLastNode(GeomBaseNode* last);

        // ��һ��� ���� (���ϴ� ��)
        void setNumberOfElements(int32 ne);
        int32 getNumberOfElements() const;

        // ���� ���� 
        void setMaterialNode(Node* cs);
        Node* getMaterialNode() const;

		// ������� ����
		bool applyBC(uint64 index, BCNode* bc);


    public:
        virtual Polygon_set_2* getPolyset();
        virtual const Curves& getCurves();
        virtual const Vertices& getVertices();

        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();
        virtual void onAttachTo(Node* parent);
        virtual void onDetachFrom(Node* parent);

    public:
        static void bindMethod();
        static void bindProperty();


    protected:
        bool bStandalone_;
        bool bModelNode_;
        Color color_;
        bool hide_;
        int32 num_elements_;

		// ���� ����� Geometry ���
        Ref<GeomBaseNode> lastNode_;

		// ���� ����
        Ref<MaterialNode> material_;

		// �������
		BCs BCs_;

    public:
        static float64 tolerance_;
    };

#include "geomheadnode.inl"

}
}