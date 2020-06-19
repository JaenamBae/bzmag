#include "geomcoverlinenode.h"
//#include "geomheadnode.h"

#include "core/vector2.h"
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
                const X_monotone_curve_2& curve = *iter;
                //X_monotone_curve_2 regen_curve = regenerate_curve(curve);
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
X_monotone_curve_2 GeomCoverLineNode::regenerate_curve(const X_monotone_curve_2& curve)
{
    Traits_2::Point_2 source = curve.source();
    Traits_2::Point_2 target = curve.target();

    // Debuging과 Traits_2::Point_2 --> Point_2 변환을 위해 필요함
    Vector2 vs(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 vt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    // CGAL::to_double() 자체가 approximate 함수임
    Point_2 p1(vs.x_, vs.y_);
    Point_2 p2(vt.x_, vt.y_);
    Point_2 p3;

    Curve_2 regen_curve;

    // 커브가 arc이면 X_monotone_curve_2를 3점(시작, 중간, 끝)으로 표현하고
    // linear이면 2점(시작, 끝)으로 표현함
    if (curve.is_circular()) {
        // Info of supporint circle
        Circle_2 supporing_circle = curve.supporting_circle();
        Point_2 center = supporing_circle.center();
        Vector2 vc(CGAL::to_double(center.x()), CGAL::to_double(center.y()));
        double radii = sqrt(CGAL::to_double(supporing_circle.squared_radius()));

        // atan2 함수를 CGAL core를 이용해 구현할 수는 없는지...?
        float64 ang_start = atan2(vs.y_ - vc.y_, vs.x_ - vc.x_);
        float64 ang_end = atan2(vt.y_ - vc.y_, vt.x_ - vc.x_);

        float64 d_ang = (ang_end - ang_start);
        if (curve.orientation() == CGAL::COUNTERCLOCKWISE)
        {
            if (d_ang < 0) d_ang += (2 * CGAL_PI);
        }
        else
        {
            if (d_ang > 0) d_ang -= (2 * CGAL_PI);
        }
        float64 ang_mid = ang_start + d_ang * 0.5;

        Vector2 vm(vc.x_ + radii * cos(ang_mid), vc.y_ + radii * sin(ang_mid));
        p3 = Point_2(vm.x_, vm.y_);
        regen_curve = Curve_2(p1, p3, p2);
    }

    else if (curve.is_linear()) {
        regen_curve = Curve_2(p1, p2);
    }

    Traits_2 traits;
    std::list<CGAL::Object> objects;
    traits.make_x_monotone_2_object() (regen_curve, std::back_inserter(objects));

    X_monotone_curve_2 result;
    std::list<CGAL::Object>::iterator iter;
    for (iter = objects.begin(); iter != objects.end(); ++iter)
    {
        CGAL::assign(result, *iter);
        Traits_2::Point_2 ss = result.source();
        Traits_2::Point_2 tt = result.target();

        Vector2 vss(CGAL::to_double(ss.x()), CGAL::to_double(ss.y()));
        Vector2 vtt(CGAL::to_double(tt.x()), CGAL::to_double(tt.y()));

        if (vss.x_ == vs.x_ && vss.y_ == vs.y_ && vtt.x_ == vt.x_ && vtt.y_ == vt.y_)
            break;
    }

    return result;
}

//----------------------------------------------------------------------------
void GeomCoverLineNode::bindProperty()
{

}
