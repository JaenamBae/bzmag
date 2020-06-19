#pragma once

/*
Description : Abstract Node for Geometry Shapes
Last Update : 2018.06.20
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"

namespace bzmag
{
namespace engine
{
    class GeomSurfaceNode : public GeomBaseNode
    {
    public:
        GeomSurfaceNode();
        virtual ~GeomSurfaceNode();
        DECLARE_ABSTRACTCLASS(GeomSurfaceNode, GeomBaseNode);

    public:
        bool isClosed() const;

    public:
        //virtual Polygon_set_2* getGeometry();
        virtual bool hitTest(float64 x, float64 y) = 0;
        virtual bool isCoverd() const;
        virtual String description() const = 0;

    public:
        virtual bool update();

    public:
        virtual bool cover() = 0;

    protected:
        Polygon_2 construct_polygon(const Circle_2& circle);
        Polygon_2 construct_polygon(const Point_2& p1, const Point_2& p2,
            const Point_2& p3, const Point_2& p4);

    protected:
        // ´ÝÈù Çü»óÀÎÁö?
        bool closed_;
    };
}
}