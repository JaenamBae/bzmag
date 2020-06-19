#include "geommovenode.h"
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

IMPLEMENT_CLASS(GeomMoveNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomMoveNode::GeomMoveNode() : sdx_("0"), sdy_("0")
{
    uint32 key = getID();

    dx_ = new Expression();
    dy_ = new Expression();

    dx_->setKey("dx_" + std::to_string(key));
    dy_->setKey("dy_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomMoveNode::~GeomMoveNode()
{

}

//----------------------------------------------------------------------------
bool GeomMoveNode::setParameters(const String& dx, const String& dy)
{
    if (dx_.invalid() || dy_.invalid()) return false;

    if (!dx_->setExpression(dx) || !dy_->setExpression(dy)) {
        dx_->setExpression(sdx_);
        dy_->setExpression(sdy_);

        return false;
    }

    sdx_ = dx;
    sdy_ = dy;

    return update();
}

//----------------------------------------------------------------------------
float64 GeomMoveNode::get_dx() const
{
    Expression* dx = dx_;
    if (dx_.valid())
        return dx->eval();
    else
        return 0;
}

//----------------------------------------------------------------------------
float64 GeomMoveNode::get_dy() const
{
    Expression* dy = dy_;
    if (dy_.valid())
        return dy->eval();
    else
        return 0;
}

//----------------------------------------------------------------------------
void GeomMoveNode::movePolygon_with_hole(const Polygon_with_holes_2& lhs
    , Polygon_with_holes_2& rhs)
{
    rhs.clear();
    if (!lhs.is_unbounded()) {
        Polygon_2 poly;
        movePolygon(lhs.outer_boundary(), poly);
        rhs = Polygon_with_holes_2(poly);
    }
    else return;

    Polygon_with_holes_2::Hole_const_iterator hit;
    for (hit = lhs.holes_begin(); hit != lhs.holes_end(); ++hit) {
        const Polygon_2& P = *hit;
        Polygon_2 poly;
        movePolygon(P, poly);
        rhs.add_hole(poly);
    }

    return;
}

//----------------------------------------------------------------------------
//Description : �־��� ������(lhs)�� �־��� ����(dx, dy)��ŭ �̵��Ͽ� 
//              rhs�� ������
//              �̶� ������ǥ��� GeomMoveNode�� �����ϰ� �ִ� CSNode�� ��
//Comment : Move���� �߿��� ���� ���⺤����.
//          �� ������ǥ���� ������ �߽����� �� ������ �߿���
//Last update : 2017.09.28
//Author : Jaenam Bae (jaenam@dongyang.ac.kr)
//----------------------------------------------------------------------------
void GeomMoveNode::movePolygon(const Polygon_2& lhs, Polygon_2& rhs)
{
    rhs.clear();

    // Moving�� ���� ������ǥ�� ����� �������
    Vector_2 disp = displacement();

    Polygon_2::Curve_const_iterator vit;
    for (vit = lhs.curves_begin(); vit != lhs.curves_end(); ++vit) {
        Curve_2 moved_curve;
        if (!moveCurve(*vit, moved_curve, disp))
            continue;

        Traits_2 traits;
        std::list<CGAL::Object> objects;
        traits.make_x_monotone_2_object() (moved_curve, std::back_inserter(objects));

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
void  GeomMoveNode::moveCurves(const Curves& lhs, Curves& rhs)
{
    rhs.clear();

    // Moving�� ���� ������ǥ�� ����� �������
    Vector_2 disp = displacement();

    Curves::const_iterator vit;
    for (vit = lhs.begin(); vit != lhs.end(); ++vit) {
        const X_monotone_curve_2& arc = *vit;

        Curve_2 moved_curve;
        if (!moveCurve(arc, moved_curve, disp))
            continue;

        Traits_2 traits;
        std::list<CGAL::Object> objects;
        traits.make_x_monotone_2_object() (moved_curve, std::back_inserter(objects));

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
void  GeomMoveNode::moveVertices(const Vertices& lhs, Vertices& rhs)
{
    rhs.clear();

    // Moving�� ���� ������ǥ�� ����� �������
    Vector_2 disp = displacement();

    Vertices::iterator it;
    for (it = vertices_.begin(); it != vertices_.end(); ++it)
    {
        Traits_2::Point_2 v = *it;
        Point_2 vv(CGAL::to_double(v.x()), CGAL::to_double(v.y()));
        Point_2 move_vv = vv + disp;

        rhs.emplace_back(Traits_2::Point_2(move_vv.x(), move_vv.y()));
    }
}

//----------------------------------------------------------------------------
bool GeomMoveNode::moveCurve(const X_monotone_curve_2& curve, Curve_2& moved_curve, const Vector_2& disp)
{
    const X_monotone_curve_2& arc = curve;

    Traits_2::Point_2 source = arc.source();
    Traits_2::Point_2 target = arc.target();

    // Debuging�� Traits_2::Point_2 --> Point_2 ��ȯ�� ���� �ʿ���
    Vector2 vs(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 vt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    if (vs.x_ == vt.x_ && vs.y_ == vt.y_) {
        return false;
    }

    // ���Ͽ��� CGAL::to_double �� ���� �ʰ� �� �� �ִ� ����� �ִ���...?
    // CGAL::to_double() ��ü�� approximate �Լ���
    Point_2 p1(vs.x_, vs.y_);
    Point_2 p2(vt.x_, vt.y_);
    Point_2 p3;

    // Ŀ�갡 arc�̸� X_monotone_curve_2�� 3��(����, �߰�, ��)���� ǥ���ϰ�
    // linear�̸� 2��(����, ��)���� ǥ����
    if (arc.is_circular()) {
        Circle_2 supporing_circle = arc.supporting_circle();
        Point_2 center = supporing_circle.center();
        Vector2 vc(CGAL::to_double(center.x()), CGAL::to_double(center.y()));
        double radii = sqrt(CGAL::to_double(supporing_circle.squared_radius()));

        // atan2 �Լ��� CGAL core�� �̿��� ������ ���� ������...?
        float64 ang_start = atan2(vs.y_ - vc.y_, vs.x_ - vc.x_);
        float64 ang_end = atan2(vt.y_ - vc.y_, vt.x_ - vc.x_);

        // ������ ������ �������� �������� �׻� ū ���� �����ϰ� �� (����� ����)
        if (ang_start > ang_end) ang_end += 2.0*CGAL_PI;

        float64 ang_mid = 0;
        if (arc.orientation() == CGAL::COUNTERCLOCKWISE) {
            ang_mid = ang_start + (ang_end - ang_start) * 0.5;
        }
        else {
            ang_mid = ang_start - (ang_end - ang_start) * 0.5;
        }
        Vector2 vm(vc.x_ + radii * cos(ang_mid), vc.y_ + radii * sin(ang_mid));
        p3 = Point_2(vm.x_, vm.y_);

        p1 = p1 + disp;
        p2 = p2 + disp;
        p3 = p3 + disp;

        // �Ʒ����� ��û�Ѱ� ������ �ʿ��� (CGAL ���ܹ��� ����)...
        Vector2 tp1(CGAL::to_double(p1.x()), CGAL::to_double(p1.y()));
        Vector2 tp2(CGAL::to_double(p2.x()), CGAL::to_double(p2.y()));
        Vector2 tp3(CGAL::to_double(p3.x()), CGAL::to_double(p3.y()));
        p1 = Point_2(tp1.x_, tp1.y_);
        p2 = Point_2(tp2.x_, tp2.y_);
        p3 = Point_2(tp3.x_, tp3.y_);

        if (p1 == p2 || p2 == p3 || p3 == p1) return false;
        moved_curve = Curve_2(p1, p3, p2);
    }

    if (arc.is_linear()) {
        p1 = p1 + disp;
        p2 = p2 + disp;

        // �Ʒ����� ��û�Ѱ� ������ �ʿ��� (CGAL ���ܹ��� ����)...
        Vector2 tp1(CGAL::to_double(p1.x()), CGAL::to_double(p1.y()));
        Vector2 tp2(CGAL::to_double(p2.x()), CGAL::to_double(p2.y()));
        p1 = Point_2(tp1.x_, tp1.y_);
        p2 = Point_2(tp2.x_, tp2.y_);

        moved_curve = Curve_2(p1, p2);
    }

    return true;
}

//----------------------------------------------------------------------------
Vector_2 GeomMoveNode::displacement()
{
    // ������ǥ�谡 �����Ǿ� �ִٸ�, 
    Transformation trans;
    if (cs_.valid()) {
        float64 angle = cs_->getGlobalAngle();

        NT diry = std::sin(angle) * 256 * 256 * 256;
        NT dirx = std::cos(angle) * 256 * 256 * 256;
        NT sin_alpha;
        NT cos_alpha;
        NT w;
        CGAL::rational_rotation_approximation(dirx, diry,
            sin_alpha, cos_alpha, w,
            NT(1), NT(1000000));

        trans = Transformation(CGAL::ROTATION, sin_alpha, cos_alpha, w);
    }

    // ���� �̵� �� ����
    Vector_2 disp = trans(Vector_2(dx_->eval(), dy_->eval()));
    return disp;
}

//----------------------------------------------------------------------------
bool GeomMoveNode::hitTest(float64 x, float64 y)
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
bool GeomMoveNode::update()
{
    if (dx_.invalid() || dy_.invalid())
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
            // geometry_�� ��� ����Ʈ���� dx, dy��ŭ �̵��� �� �ٽ� ����� ��
            std::list<Polygon_with_holes_2> res;
            std::list<Polygon_with_holes_2>::const_iterator it;
            geometry->polygons_with_holes(std::back_inserter(res));

            Polygon_set_2* new_geometry = new Polygon_set_2;
            for (it = res.begin(); it != res.end(); ++it)
            {
                Polygon_with_holes_2 lhs = *it;
                Polygon_with_holes_2 rhs;
                movePolygon_with_hole(lhs, rhs);
                new_geometry->insert(rhs);
            }
            geometry_ = new_geometry;
        }
        else if (curves.size() > 0)
        {
            Curves rotate_curves;
            moveCurves(curves, rotate_curves);
            curves_ = rotate_curves;
        }

        else if (vertices.size() > 0)
        {
            Vertices rotate_vertices;
            moveVertices(vertices, rotate_vertices);
            vertices_ = rotate_vertices;
        }
    }

    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
void GeomMoveNode::bindProperty()
{
    BIND_PROPERTY(const String&, dx, &set_dx, &get_dx_asString);
    BIND_PROPERTY(const String&, dy, &set_dy, &get_dy_asString);
}
