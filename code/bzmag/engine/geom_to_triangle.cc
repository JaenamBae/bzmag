#include "geom_to_triangle.h"
#include "core/vector2.h"

#include <vector>
#include <iterator>
#include <CGAL/Exact_circular_kernel_2.h>
#include <CGAL/Circular_kernel_intersections.h>
#include <CGAL/intersections.h>
#include <CGAL/Boolean_set_operations_2.h>

typedef CGAL::Exact_circular_kernel_2        CircK;
typedef CGAL::Point_2<CircK>                 Pt2;
typedef CGAL::Circle_2<CircK>                Circ2;
typedef CGAL::Line_arc_2<CircK>              LineArc2;
typedef CGAL::Circular_arc_2<CircK>          CircleArc2;
typedef CGAL::CK2_Intersection_traits<CircK, Circ2, LineArc2>::type InterResLine;
typedef CGAL::CK2_Intersection_traits<CircK, Circ2, CircleArc2>::type InterResCircle;

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomToPolyTriangle, Node);

X_monotone_curve_2* GeomToPolyTriangle::current_edgy_ = nullptr;
float64 GeomToPolyTriangle::torr_ = 1e-12;

//-----------------------------------------------------------------------------
GeomToPolyTriangle::GeomToPolyTriangle()
{

}

//-----------------------------------------------------------------------------
GeomToPolyTriangle::~GeomToPolyTriangle()
{

}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::convert(const String& path)
{
    Kernel* kernel = Kernel::instance();
    Node* node = kernel->lookup(path);
    convertPolyStructure(node);
}

//-----------------------------------------------------------------------------
bool GeomToPolyTriangle::convertPolyStructure(const Node* poly_root)
{
    if (0 == poly_root) return false;

    // 모든 geometry 들이 담길 변수
    PolygonList polygons, spilted_polygons, approx_polygons;
    PointList points;

    // 모든 Geometry들로부터 기저 절점(Basis Vertecs)을 뽑아낸다
    extractPolygons(poly_root, polygons);
    extractPoints(polygons, points);

    // 모든 Geometry에 대해서 절점을 기준으로 edgy를 나눈다
    PolygonIterator pit;
    for (pit = polygons.begin(); pit != polygons.end(); ++pit)
    {
        const Polygon_2& polygon = *pit;
        Polygon_2 spilted_polygon;
        spiltPolygon(polygon, points, spilted_polygon);
        spilted_polygons.push_back(spilted_polygon);
    }

    // General Polygon --> Segmented & Approximated Polygon 
    for (pit = spilted_polygons.begin(); pit != spilted_polygons.end(); ++pit)
    {
        Polygon_2 polygon = *pit;

        // 아래 함수가 핵심
        Polygon_2 approx_poly = approximatePolygon(polygon, 5);

        approx_polygons.push_back(approx_poly);
    }

    // 중복된 폴리곤 삭제 
    // 주) 현 시점에서 모든 폴리곤은 세그멘테이션이 된 상태이며
    //     세그멘테이션 시 모든 절점은 Approximate 되었음
    removeDuplicate(approx_polygons);

    // 이제 *.POLY 파일을 쓸 준비가 끝났다.
    makeTriangleInput(approx_polygons);

    return true;
}

//-----------------------------------------------------------------------------
// Last Updated : 2017.06.10
//
void GeomToPolyTriangle::extractPolygons(const Node* poly_root, PolygonList& polygons)
{
    // STEP 1
    // 루트노드(node)하위의 head 노드를 순차적으로 검색하여 Geometry를 끄집어 낸다
    // Geometry는 polygons_with_holes 형태이므로 여러폴리곤들과 홀들로 구성된다.
    // 따라서 각 polygon_wiht_holes을 추출한 후
    // 외곽 경계와 내부 홀 경계 추출을 각 polygon_with_holes에 대하여 수행 해야 한다.
    Node::ConstNodeIterator it1;
    for (it1 = poly_root->firstChildNode(); it1 != poly_root->lastChildNode(); ++it1)
    {
        Node* childnode = *it1;
        GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(childnode);
        if (head && head->isModelNode() && head->isStandAlone())
        {
            Polygon_set_2* geometry = head->getGeometry();
            if (geometry)
            {
                // 아래줄은 무슨역할을 하는지 잘 모르겠지만 그냥 적어놨다
                geometry->remove_redundant_edges();

                // polygons_with_holes --> 리스트 형태의 polygon_with_holes로 변환
                std::list<Polygon_with_holes_2> res;
                std::list<Polygon_with_holes_2>::iterator it2;
                geometry->polygons_with_holes(std::back_inserter(res));

                // 각 polygon_wiht_holes에 대하여 외곽 경계와, 내부 홀 경계를 추출해 PolygonList에 삽입함
                for (it2 = res.begin(); it2 != res.end(); ++it2)
                {
                    // polygon_with_holes 을 모두 polygon으로 바꾸어 results에 넣는다.
                    // 이 부분은 조금 생각해 보아야 할 듯(?)
                    Polygon_with_holes_2& polyhole = *it2;

                    // 외곽 경계를 추출해 PolygonList에 삽입함
                    polygons.push_back(polyhole.outer_boundary());

                    // 내부 홀들을 추출해 PolygonList에 삽입함
                    Polygon_with_holes_2::Hole_iterator hit;
                    for (hit = polyhole.holes_begin(); hit != polyhole.holes_end(); ++hit) {
                        Polygon_2& P = *hit;

                        // 내부 홀은 방향이 반대이므로, 뒤집어서 List에 추가함
                        P.reverse_orientation();
                        polygons.push_back(P);
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Last Updated : 2017.06.10
//
void GeomToPolyTriangle::extractPoints(const PolygonList& polygons, PointList& points)
{
    // STEP 2
    // 모든 Polygon들을 이루고 있는 모든 절점을 추출한다
    // 실제로 동일점으로 인정될 수 있는 절점들이지만 아주 미세한 오차가 나는 절점들은
    // 현 단계에서는 서로 다른점으로 인식하게 한다.
    PolygonIterator pit;
    for (pit = polygons.begin(); pit != polygons.end(); ++pit)
    {
        Polygon_2::Curve_const_iterator cit;
        const Polygon_2& polygon = *pit;
        for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
        {
            const X_monotone_curve_2& edgy = *cit;

            Traits_2::Point_2 source = edgy.source();
            Traits_2::Point_2 target = edgy.target();

            // 동일 절점은 빼고 추출하기
            PointIterator f1 = std::find(points.begin(), points.end(), source);
            if (f1 == points.end())
                points.push_back(source);

            PointIterator f2 = std::find(points.begin(), points.end(), target);
            if (f2 == points.end())
                points.push_back(target);
        }
    }
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::spiltPolygon(const Polygon_2& polygon,
    const PointList& points,
    Polygon_2& spilted_polygon)
{
    spilted_polygon.clear();

    Polygon_2::Curve_const_iterator cit;
    for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
    {
        const X_monotone_curve_2& edgy = *cit;
        EdgyList spilted_edgis;

        spilt_edgy(edgy, points, spilted_edgis);
        EdgyList::iterator it;
        for (it = spilted_edgis.begin(); it != spilted_edgis.end(); ++it)
        {
            X_monotone_curve_2& spilted_edgy = *it;
            spilted_polygon.push_back(spilted_edgy);
        }
    }
}

//-----------------------------------------------------------------------------
Polygon_2 GeomToPolyTriangle::approximatePolygon(const Polygon_2& polygon, float64 base_length)
{
    Polygon_2 ouput;

    // segmentation 시 주어진 폴리곤을 이루는 엣지 중 선형엣지의 길이를 활용하는 것도 좋은 방법일듯
    float64 max_length = 0;


    // 주어진 폴리곤의 가로세로크기도 활용하면 좋을 듯
    CGAL::Bbox_2 box = polygon.bbox();
    float64 width = abs(box.xmax() - box.xmin());
    float64 height = abs(box.ymax() - box.ymin());

    Polygon_2::Curve_const_iterator cit;
    for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
    {
        const X_monotone_curve_2& edgy = *cit;

        if (edgy.is_circular())
        {
            Circle_2 circle = edgy.supporting_circle();
            Point_2 center = circle.center();
            Traits_2::Point_2 source = edgy.source();
            Traits_2::Point_2 target = edgy.target();

            Vector2 cc(approximate_double(CGAL::to_double(center.x())),
                approximate_double(CGAL::to_double(center.y())));

            Vector2 ss(approximate_double(CGAL::to_double(source.x())),
                approximate_double(CGAL::to_double(source.y())));

            Vector2 tt(approximate_double(CGAL::to_double(target.x())),
                approximate_double(CGAL::to_double(target.y())));

            float64 radii = approximate_double(sqrt(CGAL::to_double(circle.squared_radius())));

            float64 ang_start = atan2(ss.y_ - cc.y_, ss.x_ - cc.x_);
            float64 ang_end = atan2(tt.y_ - cc.y_, tt.x_ - cc.x_);
            float64 del_ang = ang_end - ang_start;
            if (del_ang < 0) del_ang += (2 * CGAL_PI);

            float64 arc_length = radii * del_ang;
            int num_seg1 = int(arc_length / base_length + 0.5);
            if (num_seg1 == 0) num_seg1 = 1;
            int num_seg2 = int(del_ang / CGAL_PI * 6 + 0.5);

            int num_seg = std::max(num_seg1, num_seg2);
            float64 dd_ang = del_ang / num_seg;
            for (int i = 1; i <= num_seg; ++i) {
                Vector2 mm(cc.x_ + radii * cos(ang_start + i*dd_ang), cc.y_ + radii * sin(ang_start + i*dd_ang));
                if (i == num_seg) mm = tt;
                
                if(ss.x_ == mm.x_ && ss.y_ == mm.y_) continue;

                X_monotone_curve_2 segmented_edgy(Point_2(ss.x_, ss.y_), Point_2(mm.x_, mm.y_));
                ouput.push_back(segmented_edgy);

                ss = mm;
            }
        }

        // X_monotone_curve_2 가 직선인 경우...
        // 일단 segmentation하지 않고 그대로 쓰기로 함
        // 나중에 코드 수정이 필요할 것 같음
        else
        {
            Traits_2::Point_2 source = edgy.source();
            Traits_2::Point_2 target = edgy.target();

            Vector2 ss(approximate_double(CGAL::to_double(source.x())),
                approximate_double(CGAL::to_double(source.y())));

            Vector2 tt(approximate_double(CGAL::to_double(target.x())),
                approximate_double(CGAL::to_double(target.y())));


            if (ss.x_ == tt.x_ && ss.y_ == tt.y_) continue;

            X_monotone_curve_2 segmented_edgy(Point_2(ss.x_, ss.y_), Point_2(tt.x_, tt.y_));
            ouput.push_back(segmented_edgy);
        }
    }

    return ouput;
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::removeDuplicate(PolygonList& polygons)
{
    if (polygons.size() > 1) {
        PolygonIterator pit;
        for (pit = polygons.begin(); pit != polygons.end(); ++pit)
        {
            Polygon_2 poly1(*pit);

            PolygonIterator pit2 = pit;
            for (++pit2; pit2 != polygons.end(); )
            {
                Polygon_2 poly2(*pit2);

                if (poly1.size() == poly2.size()) {
                    // 만약 중복되면 삭제
                    std::list<Polygon_with_holes_2> R1;
                    CGAL::symmetric_difference(poly1, poly2, std::back_inserter(R1));
                    if (R1.empty()) {
                        pit2 = polygons.erase(pit2);
                    }
                    else ++pit2;
                }
                else {
                    ++pit2;
                }
            }
        }
    }
    
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::spilt_edgy(const X_monotone_curve_2& curve,
    const PointList& points,
    EdgyList& spilted_edgies)
{
    Traits_2::Point_2 source = curve.source();
    Traits_2::Point_2 target = curve.target();
    Vector2 ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    // 주어진 포인트셋(points)중 주어진 curve 상에 존재하는 points의 집합
    PointList on_points;

    PointList::const_iterator it;
    for (it = points.begin(); it != points.end(); ++it)
    {
        const Traits_2::Point_2& pt = *it;

        // 주어진 curve의 시작포인트와 끝포인트는 평가에서 제외한다
        if (source == pt || target == pt) continue;

        if (point_on_curve(curve, pt)) {
            on_points.push_back(pt);
        }
    }

    // 주어진 curve를 on_points로 조각낸다.
    // on_points를 소팅할 필요가 있다
    // current_edgy_는 compare_points 에서 참조되므로
    // 함수 호출 전에 셋팅해 주어야 한다
    current_edgy_ = const_cast<X_monotone_curve_2*>(&curve);
    on_points.sort(compare_points);
    current_edgy_ = 0;

    // 소팅된 절점을 기준으로 edgy를 분할한다
    X_monotone_curve_2 curve_now(curve);
    for (it = on_points.begin(); it != on_points.end(); ++it)
    {
        Traits_2::Point_2 pt = *it;

        X_monotone_curve_2 edgy1, edgy2;
        edgy1 = curve_now.trim(source, pt);
        edgy2 = curve_now.trim(pt, target);

        spilted_edgies.push_back(edgy1);
        curve_now = edgy2;

        source = curve_now.source();
        target = curve_now.target();
        ss = Vector2(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
        tt = Vector2(CGAL::to_double(target.x()), CGAL::to_double(target.y()));
    }

    spilted_edgies.push_back(curve_now);
}

//-----------------------------------------------------------------------------
// CGAL::intersection 함수를 이용해 판단을 할 것이다.
// 오차범위 인정을 위해 오차 크기를 반지름으로 하는 원을 만들고 
// 그 원과 해당 커브가 intersection하면 커브상에 주어진 점이 존재한다고 판단할 것이다
// Last Updated : 2017.06.10
//
bool GeomToPolyTriangle::point_on_curve(const X_monotone_curve_2& curve, const Traits_2::Point_2& pt)
{
    Traits_2::Point_2 source = curve.source();
    Traits_2::Point_2 target = curve.target();

    Vector2 v_pt(CGAL::to_double(pt.x()), CGAL::to_double(pt.y()));
    Vector2 v_ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
    Vector2 v_tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

    if (v_ss.x_ == v_tt.x_ && v_ss.y_ == v_tt.y_) {
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

    // CGAL::intersection 함수를 사용하려고 하는데, 
    // 이 함수 사용법이 어렵다.
    // X_monotone_curve_2 데이터형을 인자로 받을수 없다. 
    // 비슷한 데이터 형으로 CircleArc2 형이 있는데 이는 인자로 받아진다.
    // X_monotone_curve_2 데이터 형을 CircleArc2 형태로 바꾸기가 쉽지 않다
    // CircleArc2 라는 데이터형의 생성자를 좀더 공부해 보아야 하겠지만,
    // 중심점, 반지름, 시작점 그리고 끝점으로 CircleArc2 데이터형을 초기화 시킬수가 없다
    // 그래서 커브상의 세점(시작점, 중심점, 끝점)을 이용한 원 만들기를 이용했다
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

        // 커브의 각도 계산 (0~360도로 정규화)
        float64 d_ang = (vt_ang - vs_ang);
        if (d_ang < 0) d_ang += CGAL_PI * 2;

        // 커브 중심점에서의 각도 계산
        float64 angle = 0;
        if (curve.orientation() == CGAL::COUNTERCLOCKWISE)
            angle = vs_ang + d_ang * 0.5;
        else
            angle = vs_ang - d_ang * 0.5;

        // 커브 중심점의 좌표계산
        float64 mx = ox + radii*cos(angle);
        float64 my = oy + radii*sin(angle);
        Pt2 mm(mx, my);

        // 세점을 이용한 아크 만들기
        std::vector<InterResCircle> output;
        CircleArc2 a(ss, mm, tt);

        // 아래 intersection 함수가 예전 코딩시(2016.05.25)에는 두번째 줄이 에러없이 컴파일 되었고
        // 지금(2016.07.25)은 첫번째 줄이 에러없이 컴파일 됨
        // 조금 이상하니 주의!!
        CGAL::intersection(a, c, std::back_inserter(output));
//      CGAL::intersection(c, a, std::back_inserter(output));

        /*
        if (output.size() > 0) {
            if (output[0].which() == 1)
            {
                const std::pair<CGAL::Circular_arc_point_2<CircK>, unsigned int>& inter_pt1
                    = boost::get<std::pair<CGAL::Circular_arc_point_2<CircK>, unsigned int>>(output[0]);
                CGAL::Circular_arc_point_2<CircK> pp1 = inter_pt1.first;

                const std::pair<CGAL::Circular_arc_point_2<CircK>, unsigned int>& inter_pt2
                    = boost::get<std::pair<CGAL::Circular_arc_point_2<CircK>, unsigned int>>(output[1]);
                CGAL::Circular_arc_point_2<CircK> pp2 = inter_pt2.first;

                Vector2 new_pt1(CGAL::to_double(pp1.x()), CGAL::to_double(pp1.y()));
                Vector2 new_pt2(CGAL::to_double(pp2.x()), CGAL::to_double(pp2.y()));

                Traits_2::Point_2 new_pt(CoordNT(new_pt1.x_), CoordNT(new_pt1.y_));

                pt = new_pt;
            }
        }
        */

        return (output.size() > 0) ? true : false;
    }
}


//-----------------------------------------------------------------------------
float64 GeomToPolyTriangle::approximate_double(float64 val)
{
    float64 new_val;

    std::stringstream precisionValue;
    precisionValue << std::fixed << val << std::endl;
    new_val = std::stod(precisionValue.str());

    return new_val;
}

//-----------------------------------------------------------------------------
void GeomToPolyTriangle::makeTriangleInput(const PolygonList& polygons)
{
    // clear previous data
    vertices_.clear();
    segments_.clear();
    holes_.clear();
    regions_.clear();

    // init IDs
    uint32 vID = 0;     // vertex ID
    uint32 sID = 0;     // segment ID
    uint32 hID = 0;     // hole ID
    uint32 rID = 0;     // region ID

    // iterations
    PolygonIterator pit;
    for (pit = polygons.begin(); pit != polygons.end(); ++pit)
    {
        const Polygon_2& polygon = *pit;
        Polygon_2::Curve_const_iterator cit;

        uint32 vID_start = vID;
        for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
        {
            const X_monotone_curve_2& edgy = *cit;
            Traits_2::Point_2 source = edgy.source();
            Traits_2::Point_2 target = edgy.target();
            Vector2 ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
            Vector2 tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

            vertices_[vID] = ss;
            segments_[sID] = Segments::mapped_type(vID, vID+1);
            
            vID = vID + 1;
            sID = sID + 1;
        }
        segments_[sID-1] = Segments::mapped_type(vID-1, vID_start);
    }
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::Vertices::const_iterator GeomToPolyTriangle::firstVertex() const
{
    return vertices_.begin();
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::Vertices::const_iterator GeomToPolyTriangle::lastVertex() const
{
    return vertices_.end();
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::Segments::const_iterator GeomToPolyTriangle::firstSegment() const
{
    return segments_.begin();
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::Segments::const_iterator GeomToPolyTriangle::lastSegment() const
{
    return segments_.end();
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::Regions::const_iterator GeomToPolyTriangle::firstRegion() const
{
    return regions_.begin();
}

//----------------------------------------------------------------------------
GeomToPolyTriangle::Regions::const_iterator GeomToPolyTriangle::lastRegion() const
{
    return regions_.end();
}


//-----------------------------------------------------------------------------
bool GeomToPolyTriangle::compare_points(const Traits_2::Point_2& first, const Traits_2::Point_2& second)
{
    // edgy 가 선형인 경우
    if (current_edgy_->is_linear())
    {
        const Traits_2::Point_2& source = current_edgy_->source();
        Vector2 vec1(CGAL::to_double(first.x() - source.x()), CGAL::to_double(first.y() - source.y()));
        Vector2 vec2(CGAL::to_double(second.x() - source.x()), CGAL::to_double(second.y() - source.y()));

        float64 d1 = vec1.x_ * vec1.x_ + vec1.y_ * vec1.y_;
        float64 d2 = vec2.x_ * vec2.x_ + vec2.y_ * vec2.y_;

#ifdef _DEBUG
        Vector2 vf(CGAL::to_double(first.x()), CGAL::to_double(first.y()));
        Vector2 vs(CGAL::to_double(second.x()), CGAL::to_double(second.y()));
        Vector2 vo(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
#endif

        return (d1 < d2) ? true : false;
    }

    // edgy가 arc인 경우
    else
    {
        Point_2 origin = current_edgy_->supporting_circle().center();
        Vector2 vec1(CGAL::to_double(first.x() - origin.x()), CGAL::to_double(first.y() - origin.y()));
        Vector2 vec2(CGAL::to_double(second.x() - origin.x()), CGAL::to_double(second.y() - origin.y()));

        float64 a1 = atan2(vec1.y_, vec1.x_);
        float64 a2 = atan2(vec2.y_, vec2.x_);
        if (a1 < 0) a1 += CGAL_PI * 2;
        if (a2 < 0) a2 += CGAL_PI * 2;

#ifdef _DEBUG
        Vector2 vf(CGAL::to_double(first.x()), CGAL::to_double(first.y()));
        Vector2 vs(CGAL::to_double(second.x()), CGAL::to_double(second.y()));
        Vector2 vo(CGAL::to_double(origin.x()), CGAL::to_double(origin.y()));
#endif

        return (a1 < a2) ? true : false;
    }
}

//-----------------------------------------------------------------------------
bool GeomToPolyTriangle::check_inside(Point_2 pt, Point_2 *pgn_begin, Point_2 *pgn_end, CGALKernel traits)
{
    if(CGAL::ON_BOUNDED_SIDE == CGAL::bounded_side_2(pgn_begin, pgn_end, pt, traits))
        return true;
    else
        return false;
}

//----------------------------------------------------------------------------
void GeomToPolyTriangle::bindProperty()
{

}
