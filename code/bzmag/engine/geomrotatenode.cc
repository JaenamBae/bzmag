#include "geomrotatenode.h"
#include "geomheadnode.h"
#include "geomclonefromnode.h"
#include "csnode.h"
#include "expression.h"

#include "core/vector2.h"
#include "core/kernel.h"
#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomRotateNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomRotateNode::GeomRotateNode() : sangle_("0")
{
    uint32 key = getID();

    angle_ = new Expression();
    angle_->setKey("angle_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomRotateNode::~GeomRotateNode()
{

}

//----------------------------------------------------------------------------
bool GeomRotateNode::setParameters(const String& angle)
{
    if (angle_.invalid()) return false;

    if (!angle_->setExpression(angle)) {
        angle_->setExpression(sangle_);
        return false;
    }

    sangle_ = angle;

    return update();
}

//----------------------------------------------------------------------------
float64 GeomRotateNode::getAngle() const
{
    Expression* angle = angle_;
    if (angle_.valid())
        return angle->eval();
    return 0;
}

//----------------------------------------------------------------------------
void GeomRotateNode::rotatePolygon_with_hole(const Polygon_with_holes_2& lhs
    , Polygon_with_holes_2& rhs)
{
    rhs.clear();
    if (!lhs.is_unbounded()) {
        Polygon_2 poly;
        rotatePolygon(lhs.outer_boundary(), poly);
        rhs = Polygon_with_holes_2(poly);
    }
    else return;

    Polygon_with_holes_2::Hole_const_iterator hit;
    for (hit = lhs.holes_begin(); hit != lhs.holes_end(); ++hit) {
        const Polygon_2& P = *hit;
        Polygon_2 poly;
        rotatePolygon(P, poly);
        rhs.add_hole(poly);
    }

    return;
}

//----------------------------------------------------------------------------
//Comment : Rotate에서 중요한 것은 기준점(Origin)임
//          즉 기준좌표계의 원점이 중요함
void GeomRotateNode::rotatePolygon(const  Polygon_2& lhs, Polygon_2& rhs)
{
    rhs.clear();
    Transformation transform = rotate_transform();

    Polygon_2::Curve_const_iterator vit;
    for (vit = lhs.curves_begin(); vit != lhs.curves_end(); ++vit) {
        const X_monotone_curve_2& arc = *vit;
        
        Curve_2 rotate_curve;
        if (!rotateCurve(arc, rotate_curve, transform))
            continue;

        Traits_2 traits;
        std::list<CGAL::Object> objects;
        traits.make_x_monotone_2_object() (rotate_curve, std::back_inserter(objects));

        X_monotone_curve_2 curve;
        std::list<CGAL::Object>::iterator iter;
        for (iter = objects.begin(); iter != objects.end(); ++iter)
        {
            CGAL::assign(curve, *iter);
            rhs.push_back(curve);
        }

    }

    if (lhs.orientation() != rhs.orientation())
        rhs.reverse_orientation();
}

//----------------------------------------------------------------------------
//Comment : Rotate에서 중요한 것은 기준점(Origin)임
//          즉 기준좌표계의 원점이 중요함
void GeomRotateNode::rotateCurves(const Curves& lhs, Curves& rhs)
{
    rhs.clear();
    Transformation transform = rotate_transform();

    Curves::const_iterator vit;
    for (vit = lhs.begin(); vit != lhs.end(); ++vit) {
        const X_monotone_curve_2& arc = *vit;

        Curve_2 rotate_curve;
        if (!rotateCurve(arc, rotate_curve, transform))
            continue;

        Traits_2 traits;
        std::list<CGAL::Object> objects;
        traits.make_x_monotone_2_object() (rotate_curve, std::back_inserter(objects));

        X_monotone_curve_2 curve;
        std::list<CGAL::Object>::iterator iter;
        for (iter = objects.begin(); iter != objects.end(); ++iter)
        {
            CGAL::assign(curve, *iter);
            rhs.push_back(curve);
        }
    }
}

//----------------------------------------------------------------------------
bool GeomRotateNode::rotateCurve(const X_monotone_curve_2& curve, Curve_2& rotate_curve, const Transformation& transform)
{
    const X_monotone_curve_2& arc = curve;

    Traits_2::Point_2 source = arc.source();
    Traits_2::Point_2 target = arc.target();

    // Debuging과 Traits_2::Point_2 --> Point_2 변환을 위해 필요함
    Vector2 vs(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 vt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    if (vs.x_ == vt.x_ && vs.y_ == vt.y_) {
        return false;
    }

    // 이하에서 CGAL::to_double 을 쓰지 않고 할 수 있는 방법이 있는지...?
    // CGAL::to_double() 자체가 approximate 함수임
    Point_2 p1(vs.x_, vs.y_);
    Point_2 p2(vt.x_, vt.y_);
    Point_2 p3;

    // 커브가 arc이면 X_monotone_curve_2를 3점(시작, 중간, 끝)으로 표현하고
    // linear이면 2점(시작, 끝)으로 표현함
    if (arc.is_circular()) {
        // Info of supporint circle
        Circle_2 supporing_circle = arc.supporting_circle();
        Point_2 center = supporing_circle.center();
        Vector2 vc(CGAL::to_double(center.x()), CGAL::to_double(center.y()));
        double radii = sqrt(CGAL::to_double(supporing_circle.squared_radius()));

        // atan2 함수를 CGAL core를 이용해 구현할 수는 없는지...?
        float64 ang_start = atan2(vs.y_ - vc.y_, vs.x_ - vc.x_);
        float64 ang_end = atan2(vt.y_ - vc.y_, vt.x_ - vc.x_);

        float64 d_ang = (ang_end - ang_start);
        if (arc.orientation() == CGAL::COUNTERCLOCKWISE)
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

        p1 = transform(p1);
        p2 = transform(p2);
        p3 = transform(p3);
        center = transform(center);

        // 아래줄이 멍청한것 같지만 필요함 (CGAL 예외방지 차원)...
        Vector2 tp1(CGAL::to_double(p1.x()), CGAL::to_double(p1.y()));
        Vector2 tp2(CGAL::to_double(p2.x()), CGAL::to_double(p2.y()));
        Vector2 tp3(CGAL::to_double(p3.x()), CGAL::to_double(p3.y()));
        Vector2 tpc(CGAL::to_double(center.x()), CGAL::to_double(center.y()));

        p1 = Point_2(tp1.x_, tp1.y_);
        p2 = Point_2(tp2.x_, tp2.y_);
        p3 = Point_2(tp3.x_, tp3.y_);

        // 아래에서 uncertain_conversion_exception 예외 뜸 (update 할 때만)
        if (p1 == p2 || p2 == p3 || p3 == p1) return false;
        rotate_curve = Curve_2(p1, p3, p2);
    }

    else if (arc.is_linear()) {
        p1 = transform(p1);
        p2 = transform(p2);

        // 아래줄이 멍청한것 같지만 필요함 (CGAL 예외방지 차원)...
        Vector2 tp1(CGAL::to_double(p1.x()), CGAL::to_double(p1.y()));
        Vector2 tp2(CGAL::to_double(p2.x()), CGAL::to_double(p2.y()));
        p1 = Point_2(tp1.x_, tp1.y_);
        p2 = Point_2(tp2.x_, tp2.y_);

        // 아래에서 uncertain_conversion_exception 예외 뜸 (update 할 때만)
        rotate_curve = Curve_2(p1, p2);
    }
    return true;
}

//----------------------------------------------------------------------------
Transformation GeomRotateNode::rotate_transform()
{
    Transformation trans;
    if (cs_.valid()) trans = cs_->transformation();

    // 절대좌표계상의 기준 좌표계의 원점 계산
    Point_2 org = trans(Point_2(0, 0));

    // 주어진 회전각
    float64 angle = angle_->eval();

    // 실제 Rotate 명령을 위한 변환식
    NT diry = std::sin(angle) * 256 * 256 * 256;
    NT dirx = std::cos(angle) * 256 * 256 * 256;
    NT sin_alpha;
    NT cos_alpha;
    NT w;
    CGAL::rational_rotation_approximation(dirx, diry,
        sin_alpha, cos_alpha, w,
        NT(1), NT(1000000));

    Transformation rotate(CGAL::ROTATION, sin_alpha, cos_alpha, w);

    // 참조 좌표계를 고려하기 위한 변환식
    Transformation translate1(CGAL::TRANSLATION, Vector_2(org.x(), org.y()));
    Transformation translate2(CGAL::TRANSLATION, Vector_2(-org.x(), -org.y()));

    // 아래 좌표변환식은 한번더 고민해 봐야 함
    Transformation transform = translate1 * (rotate * translate2);

    return transform;
}

//----------------------------------------------------------------------------
bool GeomRotateNode::hitTest(float64 x, float64 y)
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
bool GeomRotateNode::update()
{
    if (angle_.invalid())
        return false;

    if (geometry_) delete geometry_;
    geometry_ = nullptr;

    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (parent) {
        Polygon_set_2* geometry = parent->getPolyset();
        const Curves& curves = parent->getCurves();
        const Vertices& vertices = parent->getVertices();

        if (geometry)
        {
            // geometry_의 각 edge를 회전시킨 후 새로운 geometry를 만들어 냄
            std::list<Polygon_with_holes_2> res;
            std::list<Polygon_with_holes_2>::const_iterator it;
            geometry->polygons_with_holes(std::back_inserter(res));

            Polygon_set_2* new_geometry = new Polygon_set_2;
            for (it = res.begin(); it != res.end(); ++it)
            {
                Polygon_with_holes_2 lhs = *it;
                Polygon_with_holes_2 rhs;
                rotatePolygon_with_hole(lhs, rhs);

                if (!rhs.is_unbounded())
                    new_geometry->insert(rhs);
            }
            geometry_ = new_geometry;
        }
    }


    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
void GeomRotateNode::bindProperty()
{
    BIND_PROPERTY(const String&, Angle, &setAngle, &getAngleAsString);
}
