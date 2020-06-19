#include "geomsurfacenode.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_ABSTRACTCLASS(GeomSurfaceNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomSurfaceNode::GeomSurfaceNode() : closed_(false)
{
    //type_ = GeomBaseNode::SURFACE;
}

//----------------------------------------------------------------------------
GeomSurfaceNode::~GeomSurfaceNode()
{

}

//----------------------------------------------------------------------------
Polygon_2 GeomSurfaceNode::construct_polygon(const Circle_2& circle)
{
    // Subdivide the circle into two x-monotone arcs.
    Traits_2 traits;
    Curve_2 curve(circle);
    std::list<CGAL::Object>  objects;
    traits.make_x_monotone_2_object() (curve, std::back_inserter(objects));
    CGAL_assertion(objects.size() == 2);

    // Construct the polygon.
    Polygon_2 poly;
    X_monotone_curve_2 arc;
    std::list<CGAL::Object>::iterator iter;

    for (iter = objects.begin(); iter != objects.end(); ++iter) {
        CGAL::assign(arc, *iter);
        poly.push_back(arc);
    }
    return poly;
}

//----------------------------------------------------------------------------
Polygon_2 GeomSurfaceNode::construct_polygon(const Point_2& p1, const Point_2& p2,
    const Point_2& p3, const Point_2& p4)
{
    Polygon_2 pgn;

    X_monotone_curve_2 s1(p1, p2);    pgn.push_back(s1);
    X_monotone_curve_2 s2(p2, p3);    pgn.push_back(s2);
    X_monotone_curve_2 s3(p3, p4);    pgn.push_back(s3);
    X_monotone_curve_2 s4(p4, p1);    pgn.push_back(s4);

    return pgn;
}

//----------------------------------------------------------------------------
/*
Polygon_set_2* GeomSurfaceNode::getGeometry()
{
    return GeomBaseNode::getGeometry();
}
*/

//----------------------------------------------------------------------------
bool GeomSurfaceNode::isCoverd() const
{
    return false;
}

//----------------------------------------------------------------------------
bool GeomSurfaceNode::isClosed() const
{
    return closed_;
}

//----------------------------------------------------------------------------
bool GeomSurfaceNode::update()
{
    closed_ = cover();
    return GeomBaseNode::update();
}

