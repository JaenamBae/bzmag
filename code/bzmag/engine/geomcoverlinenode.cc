#include "geomcoverlinenode.h"
//#include "geomheadnode.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;


IMPLEMENT_CLASS(GeomCoverLineNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomCoverLineNode::GeomCoverLineNode()
{

}

//----------------------------------------------------------------------------
GeomCoverLineNode::~GeomCoverLineNode()
{

}

//----------------------------------------------------------------------------
bool GeomCoverLineNode::hitTest(float64 x, float64 y)
{
    Polygon_set_2* geometry = getPolyset();
    if (geometry) {
        Polygon_with_holes_2 poly;
        if (geometry->locate(
            Traits_2::Point_2(CoordNT(x), CoordNT(y)), poly))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------
bool GeomCoverLineNode::update()
{
    if (geometry_) delete geometry_;
    geometry_ = nullptr;

    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (parent) {
        const Curves& curves = parent->getCurves();
        if (check_closed(curves))
        {
            // Construct the polygon.
            Polygon_2 poly;
            Curves::const_iterator iter;
            for (iter = curves.begin(); iter != curves.end(); ++iter) {
                X_monotone_curve_2 curve = *iter;
                poly.push_back(curve);
            }
            geometry_ = new Polygon_set_2(poly);
        }
    }

    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
bool GeomCoverLineNode::check_closed(const Curves& curves)
{
    if (curves.size() < 2) return false;

    Curves::const_iterator iter = curves.begin();
    X_monotone_curve_2 c1 = *iter;
    Traits_2::Point_2 ss = c1.source();
    Traits_2::Point_2 tt = c1.target();
    for (iter++; iter != curves.end(); ++iter)
    {
        // 현재 커브의 끝점과 다음 커브의 시작점이 일치하는지 판단!
        const X_monotone_curve_2& c2 = *iter;
        Traits_2::Point_2 s = c2.source();
        Traits_2::Point_2 t = c2.target();

        if (tt != s) return false;

        tt = t;
    }
    if (tt != ss) return false;
    return true;
}

//----------------------------------------------------------------------------
void GeomCoverLineNode::bindProperty()
{

}
