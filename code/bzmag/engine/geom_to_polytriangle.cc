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

    // 기존 데이터 클리어
    based_polyholes_.clear();
    based_points_.clear();
    //assisted_points_.clear();

    // Polygon_with_holes 추출
    makePolyHoles(node);

    // 모든 Geometry들로부터 기저 절점(Basis Vertices)을 뽑아낸다
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

    // intersect 된 poly_holes 이 있는지 체크, 동시에 overlap 체크해 spilt-up 준비하기
    checkPolyHoles();
    //if(!checkPolyHoles()) return false;

    // overlap 된 Polygon_with_holes을 독립된 Polygon_with_holes 로 만들기
    spiltupPolyHoles();

    // Triangle Input data 생성
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& polyhelper = *it;
        makeTriangleInput(polyhelper);
    }

    // 요소생성 조건 설정
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
                // 아래줄은 무슨역할을 하는지 잘 모르겠지만 그냥 적어놨다
                geometry->remove_redundant_edges();

                // polygons_with_holes --> 리스트 형태의 polygon_with_holes로 변환
                std::list<Polygon_with_holes_2> res;
                geometry->polygons_with_holes(std::back_inserter(res));

                // 각 polygon_wiht_holes에 대하여  Based PolyHoles 생성
                std::list<Polygon_with_holes_2>::iterator it2;
                for (it2 = res.begin(); it2 != res.end(); ++it2)
                {
                    Polygon_with_holes_2& polyhole = *it2;

                    PolyHolesHelper polyholes;
                    polyholes.ID_ = head->getID();
                    polyholes.polyholes_ = polyhole;

                    // num_element_는 추후 setupMeshConditions() 에서 재계산 될 것임
                    // 하지만 초기 설정값을 알아야 하기에 일단 헤드에서 설정한 값을 복사함
                    // 추가 : 2019.04.03
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
        // polygon_with_holes 을 모두 polygon으로 바꾸어 results에 넣는다.
        const PolyHolesHelper& ph = *it;
        const Polygon_with_holes_2& polyholes = ph.polyholes_;

        // 외곽경계 추출
        const Polygon_2& outer = polyholes.outer_boundary();

        // 경계로부터 Point 추출
        extractBasedPointsFromPolygon(outer);

        // 내부 홀들로 부터 Point 추출
        Polygon_with_holes_2::Hole_const_iterator hit;
        for (hit = polyholes.holes_begin(); hit != polyholes.holes_end(); ++hit)
        {
            const Polygon_2& inner = *hit;
            extractBasedPointsFromPolygon(inner);
        }
    }

    // 중복 제거
    based_points_.sort(Point_2_sort());
    based_points_.unique();
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::extractBasedPointsFromPolygon(const Polygon_2& polygon)
{
    Polygon_2::Curve_const_iterator it;
    for (it = polygon.curves_begin(); it != polygon.curves_end(); ++it)
    {
        // 기저 Edge 추출
        const X_monotone_curve_2 curve = *it;

        // 기저 Point 추출
        Traits_2::Point_2 ss = curve.source();
        Traits_2::Point_2 tt = curve.target();
        based_points_.emplace_back(ss);
        based_points_.emplace_back(tt);

        double ssx = CGAL::to_double(ss.x());
        double ssy = CGAL::to_double(ss.y());

        double ttx = CGAL::to_double(tt.x());
        double tty = CGAL::to_double(tt.y());

        /*
        // 보조 Point 추출(커브의 센터)
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
    // 외곽경계 추출
    const Polygon_2& outer = polyholes.outer_boundary();
    Polygon_2 regen_outer = regeneratePolygonWithBasedPoints(outer);
    Polygon_with_holes_2 result(regen_outer);

    // 내부 홀들 추출
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

        // 주어진 curve를 onpoints로 조각낸다.
        // onpoints를 소팅할 필요가 있다
        // 소팅은 CCW/CW 방향성에 맞게 알아서 됨
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
// CGAL::intersection 함수를 사용하려고 하는데, 
// 이 함수 사용법이 어렵다.
// X_monotone_curve_2 데이터형을 인자로 받을수 없다. 
// 비슷한 데이터 형으로 CircleArc2 형이 있는데 이는 인자로 받아진다.
// X_monotone_curve_2 데이터 형을 CircleArc2 형태로 바꾸기가 쉽지 않다
// CircleArc2 라는 데이터형의 생성자를 좀더 공부해 보아야 하겠지만,
// 중심점, 반지름, 시작점 그리고 끝점으로 CircleArc2 데이터형을 초기화 시킬수가 없다
// 그래서 커브상의 세점(시작점, 중심점, 끝점)을 이용한 원 만들기를 이용했다
bool GeomToPolyTriangle::pointOnCurve(const X_monotone_curve_2& curve,
    const Traits_2::Point_2& pt) const
{
    Traits_2::Point_2 source = curve.source();
    Traits_2::Point_2 target = curve.target();

    Vector2 v_pt(CGAL::to_double(pt.x()), CGAL::to_double(pt.y()));
    Vector2 v_ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 v_tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    // 커브의 시작점 끝점 동일 시 실패 --> 쓰래기커브라고 판단
    float64 diff_x = (v_ss.x_ - v_tt.x_);
    float64 diff_y = (v_ss.y_ - v_tt.y_);
    float64 torr = sqrt(diff_x*diff_x + diff_y*diff_y);
    if (torr < torr_) {
        return false;
    }


    // source 및 target와 pt 사이의 거리
    Vector2 d_st(v_ss.x_ - v_pt.x_, v_ss.y_ - v_pt.y_);
    Vector2 d_tt(v_tt.x_ - v_pt.x_, v_tt.y_ - v_pt.y_);

    float64 dist_st = d_st.x_ * d_st.x_ + d_st.y_ * d_st.y_;
    float64 dist_tt = d_tt.x_ * d_tt.x_ + d_tt.y_ * d_tt.y_;
    dist_st = sqrt(dist_st);
    dist_tt = sqrt(dist_tt);

    // pt가 curve의 시작점 혹은 끝점과 오차 범위 내에 있다면
    // on point로 인정하지 않음
    if ((dist_st < torr_) || (dist_tt < torr_)) {
        return false;
    }

    // 오차범위 인정을 위한 포인트에 해당하는 원
    Circ2 c(Pt2(v_pt.x_, v_pt.y_), torr_*torr_);

    // curve의 시작점 끝점
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
        // 커브의 Supporting Circle 추출
        Circle_2 circle = curve.supporting_circle();

        // Supporting Circle의 반지름, 중심점 추출
        float64 squared_radii = CGAL::to_double(circle.squared_radius());
        float64 radii = sqrt(squared_radii);
        Point_2 org = circle.center();
        float64 ox = CGAL::to_double(org.x());
        float64 oy = CGAL::to_double(org.y());

        // 커브의 중심과 커브의 시작점간 좌표를 이용한 
        // 커브(아크)의 시작점 각도 계산
        float64 vsx = v_ss.x_ - ox;
        float64 vsy = v_ss.y_ - oy;
        float64 vs_ang = atan2(vsy, vsx);

        // 커브의 중심점과 커브의 끝점간 좌표를 이용한
        // 커브(아크)의 끝점 각도 계산
        float64 vtx = v_tt.x_ - ox;
        float64 vty = v_tt.y_ - oy;
        float64 vt_ang = atan2(vty, vtx);

        // 커브의 각도 계산
        float64 d_ang = (vt_ang - vs_ang);
       
        // 커브 중심점에서의 각도 계산
        if (curve.orientation() == CGAL::COUNTERCLOCKWISE)
        {
            if (d_ang < 0) d_ang += (2 * CGAL_PI);
        }
        else
        {
            if (d_ang > 0) d_ang -= (2 * CGAL_PI);
        }
        float64 angle = vs_ang + d_ang * 0.5;

        // 커브 중심점의 좌표계산
        float64 mx = ox + radii*cos(angle);
        float64 my = oy + radii*sin(angle);
        Pt2 mm(mx, my);

        // 세점을 이용한 아크 만들기
        if (curve.orientation() != CGAL::COUNTERCLOCKWISE)
            std::swap(ss, tt);
        CircleArc2 a(ss, mm, tt);


        // 아래 intersection 함수가 예전 코딩시(2016.05.25)에는 두번째 줄이 에러없이 컴파일 되었고
        // 지금(2016.07.25)은 첫번째 줄이 에러없이 컴파일 됨
        // 조금 이상하니 주의!!
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



    // 소팅된 절점을 기준으로 edge를 분할한다
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
    // 외곽경계 추출
    const Polygon_2& outer = polyholes.outer_boundary();
    Polygon_2 approx_outer = approximatePolygon(outer, base_length);
    Polygon_with_holes_2 result(approx_outer);

    // 내부 홀들 추출
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

    // segmentation 시 주어진 폴리곤을 이루는 엣지 중 선형엣지의 길이를 활용하는 것도 좋은 방법일듯
    float64 max_length = 0;


    // 주어진 폴리곤의 가로세로크기도 활용하면 좋을 듯
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


            // based_length로 쪼갠 세그먼트 갯수와 와 10도 간격으로 쪼갠 세그먼트 갯수 중
            // 많은 것을 세그먼트 갯수로 한다
            // 세그먼트 갯수는 짝수로 한다 (대칭성 강조 위함)
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

        // X_monotone_curve_2 가 직선인 경우...
        // 일단 segmentation하지 않고 그대로 쓰기로 함
        // -->  Segmentation 함 (수정 : 2019.04.01)
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
            
            // 벡터 길이
            float64 line_length = sqrt((ss.x_ - tt.x_)*(ss.x_ - tt.x_)
                                     + (ss.y_ - tt.y_)*(ss.y_ - tt.y_));
            int num_seg = (int)(line_length / base_length  + 0.5);
            if(num_seg < 1) num_seg = 1;
            if (num_seg % 2 == 1) num_seg = num_seg + 1;

            // 라인을 세그멘테이션 한다
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
    // 디버깅용 코드
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
        // 이하코드 수정했음(2018.06.27) CW의 Polygon은 초기화 불가능
        hole.reverse_orientation();
        if (getRandomPointInPolygon(hole, pt)) {
            polyholes.holepoints_.emplace_back(pt);
        }
    }

    
    // Polylist에는 여러개의 폴리곤(정방향, 역방향)이 존재하는데, 
    // 폴리곤을 구성하는 외곽선의 방향에 따라 홀(CW)인지 아닌지(CCW) 판단함
    std::list<Polygon_2>::const_iterator pit;
    polyholes.area_polyhole_ = 0;
    for (pit = polylist.begin(); pit != polylist.end(); ++pit)
    {
        // 폴리곤 면적 계산을 위함
        // General Polygon_2 에서는 area() method를 제공해 주지 않아
        // CGAL::Polygon_2 데이터 형을 이용해 area() method를 콜 할 것임
        // 지저분 하지만, 지금은 이것이 최선임 
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

            // 폴리곤을 이루는 점들 삽입 (시작점만 삽입)
            poly_area.push_back(Pt2(ss.x_, ss.y_));

            polyholes.vertices_[vID] = ss;
            polyholes.segments_[sID] = Segments::mapped_type(vID, vID + 1);

            vID = vID + 1;
            sID = sID + 1;
        }
        
        polyholes.segments_[sID - 1] = Segments::mapped_type(vID - 1, vID_start);

        // 면적 계산하여 .polyhoes.area_ 에 저장함
        polyholes.area_polyhole_ += CGAL::to_double(poly_area.area());
    }

    // 메시 제너레이션 후
    // inpoint 찾기
}

//----------------------------------------------------------------------------
bool GeomToPolyTriangle::getRandomPointInPolygon(const Polygon_2& polygon, Vector2& pt) const
{
    // (내부 홀이 없는) 폴리곤을 Constrained_Delaunay_triangulation 을 통해
    // 요소를 생성 한 후, 임의의 삼각형의 중심점을 리턴하기로 함
    // 이때 삼각형의 중심이 폴리곤의 내부에 있지 않을 경우, 내부에 있는 다른 점을 찾을때 까지
    // 반복하여 특정 포인트를 찾아냄
    // 수정 : 2019.04.02
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

    // 요소생성
    t.insert(based_vertices.begin(), based_vertices.end());

    // 요소의 중심점이 폴리곤 내부에 있으면 true와 해당 포인트(중심점) 리턴
    for(Face_circulator fiter = t.finite_faces_begin(); fiter!=t.finite_faces_end(); ++fiter)
    {
        Point p1 = fiter->vertex(0)->point();
        Point p2 = fiter->vertex(1)->point();
        Point p3 = fiter->vertex(2)->point();

        // 중심점 리턴하기 위해 참조 인자에 값을 씀
        // (함수 리턴값이 false이면 아무 소용 없음)
        pt.x_ = (CGAL::to_double(p1.x() + p2.x() + p3.x())) / 3.0;
        pt.y_ = (CGAL::to_double(p1.y() + p2.y() + p3.y())) / 3.0;

        Polygon_with_holes_2 temp;
        Polygon_set_2 test_poly(polygon);
        Traits_2::Point_2 pp(CoordNT(pt.x_), CoordNT(pt.y_));
        if(test_poly.locate(pp, temp)) return true;
    }
    // 폴리곤 내부에 존재하는 포인트를 찾지못하면 false 리턴
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

        // 이하 locate는 경계를 포함한다
        // 나는 경계를 포함하지 않았으면 좋겠다
        // 수정요망. 2019.04.01
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
// 최 외곽 Boundary Box 의 너비를 based_elements_num_개의 정사각형으로 나눈다고 가정했을 때 
// 정사각형의 한변의 길이를 based length로 설정함
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
// 요소생성 컨디션 생성
// 추가 2019.04.03
void GeomToPolyTriangle::setupMeshConditions()
{
    float64 totalArea = 0;
    std::map<uint32, float64> areaRegion;
    PolyHolesList::iterator it;

    // HeadNode ID별 차지하는 면적을 구한다
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& poly = *it;
        areaRegion[poly.ID_] += poly.area_polyhole_;
        totalArea += poly.area_polyhole_;
    }

    // HeadNode ID별 요구하는 요소갯수를 구한다
    // 동일 ID를 가지는 based_polyhole 이 존재할 수 있기 때문에
    // 면적에 비례하여 num_elements 재 설정
    for (it = based_polyholes_.begin(); it != based_polyholes_.end(); ++it)
    {
        PolyHolesHelper& poly = *it;

        if(poly.area_polyhole_ <= 0) continue;
        if(poly.num_elements_ == 0) 
        {
            poly.num_elements_ = (uint32)(poly.area_polyhole_ / totalArea * based_elements_num_);
        }

        // 요구 요소면적
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
    // edge 가 선형인 경우
    if (current_edge_.is_linear())
    {
        const Traits_2::Point_2& source = current_edge_.source();
        Vector2 vec1(CGAL::to_double(lhs.x() - source.x()), CGAL::to_double(lhs.y() - source.y()));
        Vector2 vec2(CGAL::to_double(rhs.x() - source.x()), CGAL::to_double(rhs.y() - source.y()));

        // 소스(시작점)과 lhs의 거리
        float64 d1 = vec1.x_ * vec1.x_ + vec1.y_ * vec1.y_;

        // 소스(시작점)과 rhs의 거리
        float64 d2 = vec2.x_ * vec2.x_ + vec2.y_ * vec2.y_;

#ifdef _DEBUG
        Vector2 vf(CGAL::to_double(lhs.x()), CGAL::to_double(lhs.y()));
        Vector2 vs(CGAL::to_double(rhs.x()), CGAL::to_double(rhs.y()));
        Vector2 vo(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
#endif
        // 시작점에서 먼 순으로
        return (d1 < d2) ? true : false;
    }

    // edge가 arc인 경우
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
        // X_monotone_Curve_2 의 방향성이 반시계방향이면 각도가 큰 순으로
        if(current_edge_.orientation() == CGAL::COUNTERCLOCKWISE)
            return (a1 < a2) ? true : false;

        // 시계방향이면 각도가 작은 순으로
        else
            return (a2 < a1) ? true : false;
    }
}


