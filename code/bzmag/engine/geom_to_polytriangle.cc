#include "geom_to_polytriangle.h"
#include "geomheadnode.h"

#include <CGAL/Exact_circular_kernel_2.h>
#include <CGAL/Circular_kernel_intersections.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Polygon_2_algorithms.h>
//#include <CGAL/Triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
//#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Point_2.h>

typedef CGAL::Exact_circular_kernel_2        CircK;
typedef CGAL::Point_2<CircK>                 Pt2;
typedef CGAL::Circle_2<CircK>                Circ2;
typedef CGAL::Line_arc_2<CircK>              LineArc2;
typedef CGAL::Circular_arc_2<CircK>          CircleArc2;
typedef CGAL::CK2_Intersection_traits<CircK, Circ2, LineArc2>::type InterResLine;
typedef CGAL::CK2_Intersection_traits<CircK, Circ2, CircleArc2>::type InterResCircle;
typedef CGAL::Polygon_2<CircK>               Poly2;

typedef CGAL::Constrained_Delaunay_triangulation_2<CircK>  CDT;
typedef CDT::Finite_faces_iterator                         Face_circulator;
typedef CDT::Point                                         Point;



using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomToPolyTriangle, Node);

float64 GeomToPolyTriangle::torr_ = 1e-12;
uint32 GeomToPolyTriangle::based_elements_num_ = 2000;

//-----------------------------------------------------------------------------
GeomToPolyTriangle::GeomToPolyTriangle()
{

}

//-----------------------------------------------------------------------------
GeomToPolyTriangle::~GeomToPolyTriangle()
{

}

//-----------------------------------------------------------------------------
bool GeomToPolyTriangle::makePolyStructures(const String& path)
{
    Kernel* kernel = Kernel::instance();
    Node* node = kernel->lookup(path);

    if (0 == node) return false;

    // ���� ������ Ŭ����
    based_polyholes_.clear();
    based_points_.clear();
    //assisted_points_.clear();

    // Polygon_with_holes ����
    makePolyHoles(node);

    // ��� Geometry��κ��� ���� ����(Basis Vertices)�� �̾Ƴ���
    extractBasedPointsFromPolyHoles();


    float64 based_leng = calculateBasedSegmentLength();

    // Polygon_with_hole Approximation
    PolyHolesList::iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& polyhelper = *it;
        polyhelper.regen_polyholes_
            = regeneratePolyHolesWithBasedPoints(polyhelper.polyholes_);

        //Polygon_set_2 ttt(polyhelper.regen_polyholes_);

        polyhelper.approx_polyholes_
            = approximatePolyHoles(polyhelper.regen_polyholes_, based_leng);
    }

    // intersect �� poly_holes �� �ִ��� üũ, ���ÿ� overlap üũ�� spilt-up �غ��ϱ�
    checkPolyHoles();
    //if(!checkPolyHoles()) return false;

    // overlap �� Polygon_with_holes�� ������ Polygon_with_holes �� �����
    spiltupPolyHoles();

    // Triangle Input data ����
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& polyhelper = *it;
        makeTriangleInput(polyhelper);
    }

    // ��һ��� ���� ����
    setupMeshConditions();
    
    return true;
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::PolyHolesList::const_iterator
GeomToPolyTriangle::firstPolyHoles() const
{
    return based_polyholes_.begin();
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::PolyHolesList::const_iterator
GeomToPolyTriangle::lastPolyHoles() const
{
    return based_polyholes_.end();
}

//----------------------------------------------------------------------------
const PolyHolesHelper* GeomToPolyTriangle::getPolyHoles(uint32 index)
{
    if(index > based_polyholes_.size()) return nullptr;
    return &(based_polyholes_[index]);
}

//----------------------------------------------------------------------------
uint64 GeomToPolyTriangle::getNumberOfPolyHoles() const
{
    return based_polyholes_.size();
}

//----------------------------------------------------------------------------
void GeomToPolyTriangle::getPointsOnHoles(std::list<Vector2>& result)
{
    PolyHolesList::iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        std::list<Vector2>& p = it->holepoints_;
        result.insert(result.end(), p.begin(), p.end());
    }

    std::list<Vector2>::const_iterator v;
    for (v = result.begin(); v != result.end(); )
    {
        const Vector2& vv = *v;
        if (hitTest(vv)) {
            v = result.erase(v);
        }
        else {
            ++v;
        }
    }
}
/*
//----------------------------------------------------------------------------
std::list<Vector2>::const_iterator GeomToPolyTriangle::firstAssistedPoint() const
{
    return assisted_points_.begin();
}

//----------------------------------------------------------------------------
std::list<Vector2>::const_iterator GeomToPolyTriangle::lastAssistedPoint() const
{
    return assisted_points_.end();
}
*/

//----------------------------------------------------------------------------
bool GeomToPolyTriangle::hitTest(const Vector2& pt)
{
    Traits_2::Point_2 p(CoordNT(pt.x_), CoordNT(pt.y_));
    //double xx = CGAL::to_double(p.x());
    //double yy = CGAL::to_double(p.y());

    Polygon_with_holes_2 res;

    PolyHolesList::const_iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        const Polygon_with_holes_2& polyhole = it->final_polyholes_;
        Polygon_set_2 polyset(polyhole);
        if (polyset.locate(p, res)) 
            return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
float64 GeomToPolyTriangle::getDomainArea() const
{
    float64 meshArea = 0;
    PolyHolesList::const_iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        const PolyHolesHelper& polyhelper = *it;
        meshArea += polyhelper.area_polyhole_;
    }

    return meshArea;
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::makePolyHoles(const Node* poly_root)
{
    if (0 == poly_root) return;

    Node::ConstNodeIterator it1;
    for (it1 = poly_root->firstChildNode(); it1 != poly_root->lastChildNode(); ++it1)
    {
        Node* childnode = *it1;
        GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(childnode);
        if (head && head->isModelNode() && head->isStandAlone())
        {
            Polygon_set_2* geometry = head->getPolyset();
            if (geometry)
            {
                // �Ʒ����� ���������� �ϴ��� �� �𸣰����� �׳� �������
                geometry->remove_redundant_edges();

                // polygons_with_holes --> ����Ʈ ������ polygon_with_holes�� ��ȯ
                std::list<Polygon_with_holes_2> res;
                geometry->polygons_with_holes(std::back_inserter(res));

                // �� polygon_wiht_holes�� ���Ͽ�  Based PolyHoles ����
                std::list<Polygon_with_holes_2>::iterator it2;
                for (it2 = res.begin(); it2 != res.end(); ++it2)
                {
                    Polygon_with_holes_2& polyhole = *it2;

                    PolyHolesHelper polyholes;
                    polyholes.ID_ = head->getID();
                    polyholes.polyholes_ = polyhole;

                    // num_element_�� ���� setupMeshConditions() ���� ���� �� ����
                    // ������ �ʱ� �������� �˾ƾ� �ϱ⿡ �ϴ� ��忡�� ������ ���� ������
                    // �߰� : 2019.04.03
                    polyholes.num_elements_ = head->getNumberOfElements();
                    based_polyholes_.emplace_back(polyholes);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::extractBasedPointsFromPolyHoles()
{
    //based_points_.clear();

    PolyHolesList::const_iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        // polygon_with_holes �� ��� polygon���� �ٲپ� results�� �ִ´�.
        const PolyHolesHelper& ph = *it;
        const Polygon_with_holes_2& polyholes = ph.polyholes_;

        // �ܰ���� ����
        const Polygon_2& outer = polyholes.outer_boundary();

        // ���κ��� Point ����
        extractBasedPointsFromPolygon(outer);

        // ���� Ȧ��� ���� Point ����
        Polygon_with_holes_2::Hole_const_iterator hit;
        for (hit = polyholes.holes_begin(); hit != polyholes.holes_end(); ++hit)
        {
            const Polygon_2& inner = *hit;
            extractBasedPointsFromPolygon(inner);
        }
    }

    // �ߺ� ����
    based_points_.sort(Point_2_sort());
    based_points_.unique();
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::extractBasedPointsFromPolygon(const Polygon_2& polygon)
{
    Polygon_2::Curve_const_iterator it;
    for (it = polygon.curves_begin(); it != polygon.curves_end(); ++it)
    {
        // ���� Edge ����
        const X_monotone_curve_2 curve = *it;

        // ���� Point ����
        Traits_2::Point_2 ss = curve.source();
        Traits_2::Point_2 tt = curve.target();
        based_points_.emplace_back(ss);
        based_points_.emplace_back(tt);

        double ssx = CGAL::to_double(ss.x());
        double ssy = CGAL::to_double(ss.y());

        double ttx = CGAL::to_double(tt.x());
        double tty = CGAL::to_double(tt.y());

        /*
        // ���� Point ����(Ŀ���� ����)
        if (curve.is_circular())
        {
            Point_2 center = curve.supporting_circle().center();
            Vector2 cc(approximate_double(CGAL::to_double(center.x())),
                approximate_double(CGAL::to_double(center.y())));
            assisted_points_.emplace_back(cc);
        }
        */
    }
}

//-----------------------------------------------------------------------------
Polygon_with_holes_2 
GeomToPolyTriangle::regeneratePolyHolesWithBasedPoints(const Polygon_with_holes_2& polyholes) const
{
    // �ܰ���� ����
    const Polygon_2& outer = polyholes.outer_boundary();
    Polygon_2 regen_outer = regeneratePolygonWithBasedPoints(outer);
    Polygon_with_holes_2 result(regen_outer);

    // ���� Ȧ�� ����
    Polygon_with_holes_2::Hole_const_iterator hit;
    for (hit = polyholes.holes_begin(); hit != polyholes.holes_end(); ++hit)
    {
        const Polygon_2& inner = *hit;
        Polygon_2 regen_outer = regeneratePolygonWithBasedPoints(inner);
        //regen_outer.reverse_orientation();
        result.add_hole(regen_outer);
    }

    return result;
}

//-----------------------------------------------------------------------------
Polygon_2
GeomToPolyTriangle::regeneratePolygonWithBasedPoints(const Polygon_2& polygon) const
{
    Polygon_2 result;
    Polygon_2::Curve_const_iterator cit;
    for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
    {
        PointList onpoints;
        const X_monotone_curve_2& edge = *cit;
        getBasedPointsOnCurve(edge, onpoints);

        // �־��� curve�� onpoints�� ��������.
        // onpoints�� ������ �ʿ䰡 �ִ�
        // ������ CCW/CW ���⼺�� �°� �˾Ƽ� ��
        onpoints.sort(OnPoint_2_sort(edge));

        EdgeList output;
        spiltEdgeByPoints(edge, onpoints, output);
        EdgeList::const_iterator it;
        for (it = output.begin(); it != output.end(); ++it)
        {
            const X_monotone_curve_2& spilted_edge = *it;
            result.push_back(spilted_edge);
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::getBasedPointsOnCurve(const X_monotone_curve_2& curve,
    PointList& onpoints) const
{
    PointList::const_iterator it;
    for (it = based_points_.begin(); it != based_points_.end(); ++it)
    {
        Traits_2::Point_2 pt = *it;
        if(pointOnCurve(curve, pt))
            onpoints.emplace_back(pt);
    }
}

//-----------------------------------------------------------------------------
// CGAL::intersection �Լ��� ����Ϸ��� �ϴµ�, 
// �� �Լ� ������ ��ƴ�.
// X_monotone_curve_2 ���������� ���ڷ� ������ ����. 
// ����� ������ ������ CircleArc2 ���� �ִµ� �̴� ���ڷ� �޾�����.
// X_monotone_curve_2 ������ ���� CircleArc2 ���·� �ٲٱⰡ ���� �ʴ�
// CircleArc2 ��� ���������� �����ڸ� ���� ������ ���ƾ� �ϰ�����,
// �߽���, ������, ������ �׸��� �������� CircleArc2 ���������� �ʱ�ȭ ��ų���� ����
// �׷��� Ŀ����� ����(������, �߽���, ����)�� �̿��� �� ����⸦ �̿��ߴ�
bool GeomToPolyTriangle::pointOnCurve(const X_monotone_curve_2& curve,
    const Traits_2::Point_2& pt) const
{
    Traits_2::Point_2 source = curve.source();
    Traits_2::Point_2 target = curve.target();

    Vector2 v_pt(CGAL::to_double(pt.x()), CGAL::to_double(pt.y()));
    Vector2 v_ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 v_tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    // Ŀ���� ������ ���� ���� �� ���� --> ������Ŀ���� �Ǵ�
    float64 diff_x = (v_ss.x_ - v_tt.x_);
    float64 diff_y = (v_ss.y_ - v_tt.y_);
    float64 torr = sqrt(diff_x*diff_x + diff_y*diff_y);
    if (torr < torr_) {
        return false;
    }


    // source �� target�� pt ������ �Ÿ�
    Vector2 d_st(v_ss.x_ - v_pt.x_, v_ss.y_ - v_pt.y_);
    Vector2 d_tt(v_tt.x_ - v_pt.x_, v_tt.y_ - v_pt.y_);

    float64 dist_st = d_st.x_ * d_st.x_ + d_st.y_ * d_st.y_;
    float64 dist_tt = d_tt.x_ * d_tt.x_ + d_tt.y_ * d_tt.y_;
    dist_st = sqrt(dist_st);
    dist_tt = sqrt(dist_tt);

    // pt�� curve�� ������ Ȥ�� ������ ���� ���� ���� �ִٸ�
    // on point�� �������� ����
    if ((dist_st < torr_) || (dist_tt < torr_)) {
        return false;
    }

    // �������� ������ ���� ����Ʈ�� �ش��ϴ� ��
    Circ2 c(Pt2(v_pt.x_, v_pt.y_), torr_*torr_);

    // curve�� ������ ����
    Pt2 ss(v_ss.x_, v_ss.y_);
    Pt2 tt(v_tt.x_, v_tt.y_);

    if (curve.is_linear())
    {
        std::vector<InterResLine> output;
        LineArc2 l(ss, tt);
        CGAL::intersection(c, l, std::back_inserter(output));

        return (output.size() > 0) ? true : false;
    }
    else
    {
        // Ŀ���� Supporting Circle ����
        Circle_2 circle = curve.supporting_circle();

        // Supporting Circle�� ������, �߽��� ����
        float64 squared_radii = CGAL::to_double(circle.squared_radius());
        float64 radii = sqrt(squared_radii);
        Point_2 org = circle.center();
        float64 ox = CGAL::to_double(org.x());
        float64 oy = CGAL::to_double(org.y());

        // Ŀ���� �߽ɰ� Ŀ���� �������� ��ǥ�� �̿��� 
        // Ŀ��(��ũ)�� ������ ���� ���
        float64 vsx = v_ss.x_ - ox;
        float64 vsy = v_ss.y_ - oy;
        float64 vs_ang = atan2(vsy, vsx);

        // Ŀ���� �߽����� Ŀ���� ������ ��ǥ�� �̿���
        // Ŀ��(��ũ)�� ���� ���� ���
        float64 vtx = v_tt.x_ - ox;
        float64 vty = v_tt.y_ - oy;
        float64 vt_ang = atan2(vty, vtx);

        // Ŀ���� ���� ���
        float64 d_ang = (vt_ang - vs_ang);
       
        // Ŀ�� �߽��������� ���� ���
        if (curve.orientation() == CGAL::COUNTERCLOCKWISE)
        {
            if (d_ang < 0) d_ang += (2 * CGAL_PI);
        }
        else
        {
            if (d_ang > 0) d_ang -= (2 * CGAL_PI);
        }
        float64 angle = vs_ang + d_ang * 0.5;

        // Ŀ�� �߽����� ��ǥ���
        float64 mx = ox + radii*cos(angle);
        float64 my = oy + radii*sin(angle);
        Pt2 mm(mx, my);

        // ������ �̿��� ��ũ �����
        if (curve.orientation() != CGAL::COUNTERCLOCKWISE)
            std::swap(ss, tt);
        CircleArc2 a(ss, mm, tt);


        // �Ʒ� intersection �Լ��� ���� �ڵ���(2016.05.25)���� �ι�° ���� �������� ������ �Ǿ���
        // ����(2016.07.25)�� ù��° ���� �������� ������ ��
        // ���� �̻��ϴ� ����!!
        std::vector<InterResCircle> output;
        CGAL::intersection(a, c, std::back_inserter(output));

        return (output.size() > 0) ? true : false;
    }
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::spiltEdgeByPoints(const X_monotone_curve_2& curve,
    const PointList& onpoints,
    EdgeList& output) const
{
    Traits_2::Point_2 source = curve.source();
    Traits_2::Point_2 target = curve.target();



    // ���õ� ������ �������� edge�� �����Ѵ�
    X_monotone_curve_2 curve_now(curve);
    PointList::const_iterator it;
    for (it = onpoints.begin(); it != onpoints.end(); ++it)
    {
        Traits_2::Point_2 pt = *it;

        X_monotone_curve_2 edge1, edge2;
        edge1 = curve_now.trim(source, pt);
        edge2 = curve_now.trim(pt, target);

        output.push_back(edge1);
        curve_now = edge2;

        source = curve_now.source();
        target = curve_now.target();
    }

    output.push_back(curve_now);
}

//-----------------------------------------------------------------------------
Polygon_with_holes_2
GeomToPolyTriangle::approximatePolyHoles(const Polygon_with_holes_2& polyholes,
    float64 base_length) const
{
    // �ܰ���� ����
    const Polygon_2& outer = polyholes.outer_boundary();
    Polygon_2 approx_outer = approximatePolygon(outer, base_length);
    Polygon_with_holes_2 result(approx_outer);

    // ���� Ȧ�� ����
    Polygon_with_holes_2::Hole_const_iterator hit;
    for (hit = polyholes.holes_begin(); hit != polyholes.holes_end(); ++hit)
    {
        Polygon_2 inner = *hit;
        inner.reverse_orientation();
        Polygon_2 approx_inner = approximatePolygon(inner, base_length);
        approx_inner.reverse_orientation();
        result.add_hole(approx_inner);
    }
    assert(result.is_unbounded()==false);

    return result;
}

//-----------------------------------------------------------------------------
Polygon_2
GeomToPolyTriangle::approximatePolygon(const Polygon_2& polygon,
    float64 base_length) const
{
    Polygon_2 output;

    // segmentation �� �־��� �������� �̷�� ���� �� ���������� ���̸� Ȱ���ϴ� �͵� ���� ����ϵ�
    float64 max_length = 0;


    // �־��� �������� ���μ���ũ�⵵ Ȱ���ϸ� ���� ��
    //CGAL::Bbox_2 box = polygon.bbox();
    //float64 width = abs(box.xmax() - box.xmin());
    //float64 height = abs(box.ymax() - box.ymin());

    Polygon_2::Curve_const_iterator cit;
    for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
    {
        const X_monotone_curve_2& edge = *cit;

        if (edge.is_circular())
        {
            Circle_2 circle = edge.supporting_circle();
            Point_2 center = circle.center();
            Traits_2::Point_2 source = edge.source();
            Traits_2::Point_2 target = edge.target();
            

            Vector2 cc(approximate_double(CGAL::to_double(center.x())),
                approximate_double(CGAL::to_double(center.y())));

            Vector2 ss(approximate_double(CGAL::to_double(source.x())),
                approximate_double(CGAL::to_double(source.y())));

            Vector2 tt(approximate_double(CGAL::to_double(target.x())),
                approximate_double(CGAL::to_double(target.y())));

            if (ss.x_ == tt.x_ && ss.y_ == tt.y_) {
                continue;
            }


            float64 radii = approximate_double(sqrt(CGAL::to_double(circle.squared_radius())));

            float64 ang_start = atan2(ss.y_ - cc.y_, ss.x_ - cc.x_);
            float64 ang_end = atan2(tt.y_ - cc.y_, tt.x_ - cc.x_);
            float64 del_ang = ang_end - ang_start;
            if (edge.orientation() == CGAL::COUNTERCLOCKWISE)
            {
                if (del_ang < 0) del_ang += (2 * CGAL_PI);
            }
            else
            {
                if (del_ang > 0) del_ang -= (2 * CGAL_PI);
            }


            // based_length�� �ɰ� ���׸�Ʈ ������ �� 10�� �������� �ɰ� ���׸�Ʈ ���� ��
            // ���� ���� ���׸�Ʈ ������ �Ѵ�
            // ���׸�Ʈ ������ ¦���� �Ѵ� (��Ī�� ���� ����)
            float64 arc_length = radii * abs(del_ang);
            int num_seg1 = int(arc_length / base_length + 0.5);
            if (num_seg1 == 0) num_seg1 = 2;
            int num_seg2 = int(abs(del_ang) / CGAL_PI * 36 + 0.5);
            if (num_seg2 % 2 == 1) num_seg2 = num_seg2 + 1;

            int num_seg = std::max(num_seg1, num_seg2);
            float64 dd_ang = del_ang / num_seg;

            Vector2 p1(ss);
            for (int i = 1; i < num_seg; ++i) {
                Vector2 p2(cc.x_ + radii * cos(ang_start + i*dd_ang), cc.y_ + radii * sin(ang_start + i*dd_ang));

                X_monotone_curve_2 segmented_edge(Point_2(p1.x_, p1.y_), Point_2(p2.x_, p2.y_));
                output.push_back(segmented_edge);

                p1 = p2;
            }
            Vector2 p2(tt);
            X_monotone_curve_2 segmented_edge(Point_2(p1.x_, p1.y_), Point_2(p2.x_, p2.y_));
            output.push_back(segmented_edge);
        }

        // X_monotone_curve_2 �� ������ ���...
        // �ϴ� segmentation���� �ʰ� �״�� ����� ��
        // -->  Segmentation �� (���� : 2019.04.01)
        else
        {
            Traits_2::Point_2 source = edge.source();
            Traits_2::Point_2 target = edge.target();

            Vector2 ss(approximate_double(CGAL::to_double(source.x())),
                approximate_double(CGAL::to_double(source.y())));

            Vector2 tt(approximate_double(CGAL::to_double(target.x())),
                approximate_double(CGAL::to_double(target.y())));


            if (ss.x_ == tt.x_ && ss.y_ == tt.y_) {
                continue;
            }
            
            // ���� ����
            float64 line_length = sqrt((ss.x_ - tt.x_)*(ss.x_ - tt.x_)
                                     + (ss.y_ - tt.y_)*(ss.y_ - tt.y_));
            int num_seg = (int)(line_length / base_length  + 0.5);
            if(num_seg < 1) num_seg = 1;
            if (num_seg % 2 == 1) num_seg = num_seg + 1;

            // ������ ���׸����̼� �Ѵ�
            Vector2 p1(ss);
            for (int i = 1; i < num_seg; ++i) {
                Vector2 p2(ss.x_ + (tt.x_ - ss.x_)*((float64)i / (float64)num_seg),
                           ss.y_ + (tt.y_ - ss.y_)*((float64)i / (float64)num_seg));

                X_monotone_curve_2 segmented_edge(Point_2(p1.x_, p1.y_), Point_2(p2.x_, p2.y_));
                output.push_back(segmented_edge);

                p1 = p2;
            }
            Vector2 p2(tt);
            X_monotone_curve_2 segmented_edge(Point_2(p1.x_, p1.y_), Point_2(p2.x_, p2.y_));
            output.push_back(segmented_edge);
            
        }
    }

    /*
    // ������ �ڵ�
    cit = output.curves_begin();
    X_monotone_curve_2 c1 = *cit;
    for (cit++; cit != output.curves_end(); ++cit)
    {
        X_monotone_curve_2 c2 = *cit;

        Traits_2::Point_2 pp1 = c1.target();
        Traits_2::Point_2 pp2 = c2.source();

        double pp1x = CGAL::to_double(pp1.x());
        double pp1y = CGAL::to_double(pp1.y());

        double pp2x = CGAL::to_double(pp2.x());
        double pp2y = CGAL::to_double(pp2.y());

        if (pp1 != pp2) {
            int aaadf = 1;
        }

        c1 = c2;

    }
    */

    return output;
}

//-----------------------------------------------------------------------------
float64 GeomToPolyTriangle::approximate_double(float64 val) const
{
    float64 new_val;

    std::stringstream precisionValue;
    precisionValue << std::fixed << val << std::endl;
    new_val = std::stod(precisionValue.str());

    //return new_val;
    return val;
}

//-----------------------------------------------------------------------------
bool GeomToPolyTriangle::checkPolyHoles()
{
    bool ret = true;
    PolyHolesList::iterator it1;
    for (it1 = based_polyholes_.begin(); it1 != based_polyholes_.end(); ++it1)
    {
        PolyHolesHelper& ph1 = *it1;
        const Polygon_with_holes_2& polyhole1 = ph1.approx_polyholes_;

        PolyHolesList::iterator it2;
        for (it2 = it1, ++it2; it2 != based_polyholes_.end(); ++it2)
        {
            PolyHolesHelper& ph2 = *it2;
            const Polygon_with_holes_2& polyhole2 = ph2.approx_polyholes_;

            assert(polyhole1.is_unbounded() == false);
            assert(polyhole2.is_unbounded() == false);

            Polygon_set_2 poly1(polyhole1);
            Polygon_set_2 poly2(polyhole2);
            //poly1.
            

            size_t n1 = poly1.number_of_polygons_with_holes();
            size_t n2 = poly2.number_of_polygons_with_holes();

            poly1.difference(Polygon_set_2(polyhole2));
            poly2.difference(Polygon_set_2(polyhole1));
            poly1.remove_redundant_edges();
            poly2.remove_redundant_edges();

            size_t n11 = poly1.number_of_polygons_with_holes();
            size_t n22 = poly2.number_of_polygons_with_holes();



            if (poly1.is_empty() && poly2.is_empty()) {
                //ph1.tools_.insert(polyhole2);
                continue;
            }

            else if (!poly1.is_empty() && poly2.is_empty()) {
                ph1.tools_.join(polyhole2);
            }

            else if (poly1.is_empty() && !poly2.is_empty()) {
                ph2.tools_.join(polyhole1);
            }

            else if (!poly1.is_empty() && !poly2.is_empty()) {
                ret = false;
            }
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::spiltupPolyHoles()
{
    PolyHolesList additional_polyholes;
    PolyHolesList::iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& ph = *it;

        Polygon_set_2 polyhole(ph.approx_polyholes_);
        //Polygon_set_2 polyhole(ph.regen_polyholes_);
        
        polyhole.difference(ph.tools_);

        std::list<Polygon_with_holes_2> res;
        polyhole.polygons_with_holes(std::back_inserter(res));

        if (res.size() > 0) 
        {
            ph.final_polyholes_ = res.back();
            res.pop_back();
        }
        if (res.size() > 0) 
        {
            std::list<Polygon_with_holes_2>::const_iterator ii;
            for (ii = res.begin(); ii != res.end(); ++ii)
            {
                PolyHolesHelper new_ph(ph);
                new_ph.final_polyholes_ = *ii;
                additional_polyholes.push_back(new_ph);
            }
        }
    }
    based_polyholes_.insert(based_polyholes_.end(),
        additional_polyholes.begin(),
        additional_polyholes.end());
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::makeTriangleInput(PolyHolesHelper& polyholes) const
{
    // clear previous data
    polyholes.vertices_.clear();
    polyholes.segments_.clear();

    // init IDs
    uint32 vID = 0;     // vertex ID
    uint32 sID = 0;     // segment ID

    std::list<Polygon_2> polylist;
    Polygon_with_holes_2& final_polyholes = polyholes.final_polyholes_;
    
    Polygon_2 outer = final_polyholes.outer_boundary();
    polylist.emplace_back(outer);
    
    Polygon_with_holes_2::Hole_iterator hit;
    for (hit = final_polyholes.holes_begin(); hit != final_polyholes.holes_end(); ++hit)
    {
        Polygon_2 hole = *hit;
        polylist.emplace_back(hole);

        Vector2 pt;
        // �����ڵ� ��������(2018.06.27) CW�� Polygon�� �ʱ�ȭ �Ұ���
        hole.reverse_orientation();
        if (getRandomPointInPolygon(hole, pt)) {
            polyholes.holepoints_.emplace_back(pt);
        }
    }

    
    // Polylist���� �������� ������(������, ������)�� �����ϴµ�, 
    // �������� �����ϴ� �ܰ����� ���⿡ ���� Ȧ(CW)���� �ƴ���(CCW) �Ǵ���
    std::list<Polygon_2>::const_iterator pit;
    polyholes.area_polyhole_ = 0;
    for (pit = polylist.begin(); pit != polylist.end(); ++pit)
    {
        // ������ ���� ����� ����
        // General Polygon_2 ������ area() method�� ������ ���� �ʾ�
        // CGAL::Polygon_2 ������ ���� �̿��� area() method�� �� �� ����
        // ������ ������, ������ �̰��� �ּ��� 
        // 2019.04.02
        Poly2 poly_area;

        const Polygon_2& polygon = *pit;
        Polygon_2::Curve_const_iterator cit;

        uint32 vID_start = vID;
        for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
        {
            const X_monotone_curve_2& edge = *cit;
            const Traits_2::Point_2& source = edge.source();
            const Traits_2::Point_2& target = edge.target();
            Vector2 ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            Vector2 tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

            // �������� �̷�� ���� ���� (�������� ����)
            poly_area.push_back(Pt2(ss.x_, ss.y_));

            polyholes.vertices_[vID] = ss;
            polyholes.segments_[sID] = Segments::mapped_type(vID, vID + 1);

            vID = vID + 1;
            sID = sID + 1;
        }
        
        polyholes.segments_[sID - 1] = Segments::mapped_type(vID - 1, vID_start);

        // ���� ����Ͽ� .polyhoes.area_ �� ������
        polyholes.area_polyhole_ += CGAL::to_double(poly_area.area());
    }

    // �޽� ���ʷ��̼� ��
    // inpoint ã��
}

//----------------------------------------------------------------------------
bool GeomToPolyTriangle::getRandomPointInPolygon(const Polygon_2& polygon, Vector2& pt) const
{
    // (���� Ȧ�� ����) �������� Constrained_Delaunay_triangulation �� ����
    // ��Ҹ� ���� �� ��, ������ �ﰢ���� �߽����� �����ϱ�� ��
    // �̶� �ﰢ���� �߽��� �������� ���ο� ���� ���� ���, ���ο� �ִ� �ٸ� ���� ã���� ����
    // �ݺ��Ͽ� Ư�� ����Ʈ�� ã�Ƴ�
    // ���� : 2019.04.02
    CDT t;
    std::vector<Point> based_vertices;
    Polygon_2::Curve_const_iterator cit;
    for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
    {
        const X_monotone_curve_2& curve = *cit;
        Traits_2::Point_2 source = curve.source();
        Point pt(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
        based_vertices.emplace_back(pt);
    }
    if (based_vertices.size() < 3) return false;

    // ��һ���
    t.insert(based_vertices.begin(), based_vertices.end());

    // ����� �߽����� ������ ���ο� ������ true�� �ش� ����Ʈ(�߽���) ����
    for(Face_circulator fiter = t.finite_faces_begin(); fiter!=t.finite_faces_end(); ++fiter)
    {
        Point p1 = fiter->vertex(0)->point();
        Point p2 = fiter->vertex(1)->point();
        Point p3 = fiter->vertex(2)->point();

        // �߽��� �����ϱ� ���� ���� ���ڿ� ���� ��
        // (�Լ� ���ϰ��� false�̸� �ƹ� �ҿ� ����)
        pt.x_ = (CGAL::to_double(p1.x() + p2.x() + p3.x())) / 3.0;
        pt.y_ = (CGAL::to_double(p1.y() + p2.y() + p3.y())) / 3.0;

        Polygon_with_holes_2 temp;
        Polygon_set_2 test_poly(polygon);
        Traits_2::Point_2 pp(CoordNT(pt.x_), CoordNT(pt.y_));
        if(test_poly.locate(pp, temp)) return true;
    }
    // ������ ���ο� �����ϴ� ����Ʈ�� ã�����ϸ� false ����
    return false;
    /*
    std::vector<Point_2> based_vertices;
    Polygon_2::Curve_const_iterator cit;
    for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
    {
        const X_monotone_curve_2& curve = *cit;
        Traits_2::Point_2 source = curve.source();
        Point_2 pt(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
        based_vertices.emplace_back(pt);
    }
    
    uint64 n_size = based_vertices.size();
    for (uint64 i = 0; i < n_size; i++)
    {
        Point_2 p1 = based_vertices[i];
        Point_2 p2 = based_vertices[(i + 1) % n_size];
        Point_2 p3 = based_vertices[(i + 2) % n_size];
        Traits_2::Point_2 pp(CoordNT((p1.x() + p2.x() + p3.x()) / 3.0), CoordNT((p1.y() + p2.y() + p3.y()) / 3.0));


        Polygon_set_2 geometry(polygon);
        Polygon_with_holes_2 poly;

        // ���� locate�� ��踦 �����Ѵ�
        // ���� ��踦 �������� �ʾ����� ���ڴ�
        // �������. 2019.04.01
        if (geometry.locate(pp, poly)) {
            pt.x_ = CGAL::to_double(pp.x());
            pt.y_ = CGAL::to_double(pp.y());
            return true;
        }


//         if (CGAL::bounded_side_2(based_vertices.begin(), based_vertices.end(), pp, CGALKernel()))
//         {
//             pt.x_ = CGAL::to_double(pp.x());
//             pt.y_ = CGAL::to_double(pp.y());
//             return true;
//         }
    }

    return false;*/
}

//----------------------------------------------------------------------------
// �� �ܰ� Boundary Box �� �ʺ� based_elements_num_���� ���簢������ �����ٰ� �������� �� 
// ���簢���� �Ѻ��� ���̸� based length�� ������
float64 GeomToPolyTriangle::calculateBasedSegmentLength() const
{
    float64 area = 0;
    PolyHolesList::const_iterator it;
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        Rectangle_2 bbox = (*it).polyholes_.outer_boundary().bbox();
        float64 local_area = CGAL::to_double(bbox.area());
        if (local_area > area) area = local_area;
    }
    return std::sqrt(area / (float64)based_elements_num_);
}

//----------------------------------------------------------------------------
// ��һ��� ����� ����
// �߰� 2019.04.03
void GeomToPolyTriangle::setupMeshConditions()
{
    float64 totalArea = 0;
    std::map<uint32, float64> areaRegion;
    PolyHolesList::iterator it;

    // HeadNode ID�� �����ϴ� ������ ���Ѵ�
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& poly = *it;
        areaRegion[poly.ID_] += poly.area_polyhole_;
        totalArea += poly.area_polyhole_;
    }

    // HeadNode ID�� �䱸�ϴ� ��Ұ����� ���Ѵ�
    // ���� ID�� ������ based_polyhole �� ������ �� �ֱ� ������
    // ������ ����Ͽ� num_elements �� ����
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& poly = *it;

        if(poly.area_polyhole_ <= 0) continue;
        if(poly.num_elements_ == 0) 
        {
            poly.num_elements_ = (uint32)(poly.area_polyhole_ / totalArea * based_elements_num_);
        }

        // �䱸 ��Ҹ���
        poly.area_element_ = poly.area_polyhole_ / (float64)(poly.num_elements_);
    }
}

//----------------------------------------------------------------------------
void GeomToPolyTriangle::bindProperty()
{

}




//----------------------------------------------------------------------------
bool GeomToPolyTriangle::Point_2_sort::operator ()
(const Traits_2::Point_2& lhs, const Traits_2::Point_2& rhs) const
{
    if (lhs.x() > rhs.x()) return true;
    else if (lhs.x() == rhs.x())
    {
        if (lhs.y() > rhs.y()) return true;
    }
    return false;
}

//----------------------------------------------------------------------------
bool GeomToPolyTriangle::OnPoint_2_sort::operator()
(const Traits_2::Point_2& lhs, const Traits_2::Point_2& rhs) const
{
    // edge �� ������ ���
    if (current_edge_.is_linear())
    {
        const Traits_2::Point_2& source = current_edge_.source();
        Vector2 vec1(CGAL::to_double(lhs.x() - source.x()), CGAL::to_double(lhs.y() - source.y()));
        Vector2 vec2(CGAL::to_double(rhs.x() - source.x()), CGAL::to_double(rhs.y() - source.y()));

        // �ҽ�(������)�� lhs�� �Ÿ�
        float64 d1 = vec1.x_ * vec1.x_ + vec1.y_ * vec1.y_;

        // �ҽ�(������)�� rhs�� �Ÿ�
        float64 d2 = vec2.x_ * vec2.x_ + vec2.y_ * vec2.y_;

#ifdef _DEBUG
        Vector2 vf(CGAL::to_double(lhs.x()), CGAL::to_double(lhs.y()));
        Vector2 vs(CGAL::to_double(rhs.x()), CGAL::to_double(rhs.y()));
        Vector2 vo(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
#endif
        // ���������� �� ������
        return (d1 < d2) ? true : false;
    }

    // edge�� arc�� ���
    else
    {
        Point_2 origin = current_edge_.supporting_circle().center();
        Vector2 vec1(CGAL::to_double(lhs.x() - origin.x()), CGAL::to_double(lhs.y() - origin.y()));
        Vector2 vec2(CGAL::to_double(rhs.x() - origin.x()), CGAL::to_double(rhs.y() - origin.y()));

        float64 a1 = atan2(vec1.y_, vec1.x_);
        float64 a2 = atan2(vec2.y_, vec2.x_);
        if (a1 < 0) a1 += CGAL_PI * 2;
        if (a2 < 0) a2 += CGAL_PI * 2;

#ifdef _DEBUG
        Vector2 vf(CGAL::to_double(lhs.x()), CGAL::to_double(lhs.y()));
        Vector2 vs(CGAL::to_double(rhs.x()), CGAL::to_double(rhs.y()));
        Vector2 vo(CGAL::to_double(origin.x()), CGAL::to_double(origin.y()));
#endif
        // X_monotone_Curve_2 �� ���⼺�� �ݽð�����̸� ������ ū ������
        if(current_edge_.orientation() == CGAL::COUNTERCLOCKWISE)
            return (a1 < a2) ? true : false;

        // �ð�����̸� ������ ���� ������
        else
            return (a2 < a1) ? true : false;
    }
}


