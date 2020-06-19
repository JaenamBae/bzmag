#pragma once

/*
Description : Abstract Node for Geometry Related to
Last Update : 2017.09.29
- GeomCSNode를 Ref<>로 구현함 (메모리 해제시의 문제점 해결 위해)
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geometric_entity.h"
#include "core/node.h"
#include <vector>


namespace bzmag
{
namespace engine
{
    class GeomHeadNode;
    class CSNode;
    class GeomBaseNode : public Node
    {
    public:
        GeomBaseNode();
        virtual ~GeomBaseNode();
        DECLARE_ABSTRACTCLASS(GeomBaseNode, Node);

        typedef std::vector<X_monotone_curve_2> Curves;
        typedef std::vector<Traits_2::Point_2> Vertices;

    public:
        //GeomType getGeometryType() const;
        void setReferedCS(Node* cs);
        Node* getReferedCS() const;
        GeomHeadNode* getHeadNode();

    public:
        virtual Polygon_set_2* getPolyset();
        virtual const Curves& getCurves();
        virtual const Vertices& getVertices();

        virtual bool hitTest(float64 x, float64 y) = 0;
        virtual String description() const = 0;

    public:
        virtual bool update();
        virtual void onAttachTo(Node* parent);
        virtual void onDetachFrom(Node* parent);

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        void indexingGeometry();
        void indexingPolygon(Polygon_2& poly);

    protected:
        //GeomType type_;         // Geometry type
        GeomHeadNode* head_;    // 헤드노드 포인터
        
        Polygon_set_2* geometry_;
        Curves curves_;
        Vertices vertices_;
        
        Ref<CSNode> cs_;
        bool bHead_;            // HeadNode 인지?
    };
}
}