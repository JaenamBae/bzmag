#pragma once

/*
Description : Head Node for Handling a Geometry Node
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
#include "materialnode.h"
#include "core/color.h"


namespace bzmag
{
namespace engine
{
    class GeomHeadNode : public GeomBaseNode
    {
    public:
        GeomHeadNode();
        virtual ~GeomHeadNode();
        DECLARE_CLASS(GeomHeadNode, GeomBaseNode);

    public:
        // 컬러설정
        void setColor(const Color& color);
        const Color& getColor() const;

        // Visualization을 하지 않을 것인지?
        void setHideStatus(bool hide);
        bool isHide() const;

        // Boolean 연산에 의해 참조되는 않는 노드인지?
        void setStandAlone(bool standalone);
        bool isStandAlone() const;

        // 해석에 쓰일 모델 노드인지?
        void setModelNode(bool model);
        bool isModelNode() const;

        // Head노드가 참조하는 가장 말단 자식 노드
        void setLastNode(GeomBaseNode* last);

        // 요소생성 갯수 (원하는 값)
        void setNumberOfElements(int32 ne);
        int32 getNumberOfElements() const;

        // 재질 설정 
        void setMaterialNode(Node* cs);
        Node* getMaterialNode() const;


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
        GeomBaseNode* lastNode_;// 최종 연산된 Geometry 노드

        Ref<MaterialNode> material_;

    public:
        static float64 tolerance_;
    };

#include "geomheadnode.inl"

}
}