#include "geom_converter.h"
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

IMPLEMENT_CLASS(GeometryConverter, Node);


//-----------------------------------------------------------------------------
GeometryConverter::GeometryConverter()
{

}

//-----------------------------------------------------------------------------
GeometryConverter::~GeometryConverter()
{

}

//-----------------------------------------------------------------------------
bool GeometryConverter::convert(const String& path)
{
    Kernel* kernel = Kernel::instance();
    Node* node = kernel->lookup(path);
    
    // Step1. Extract base points
    extract_base_points();
}

//-----------------------------------------------------------------------------
bool GeometryConverter::extract_base_points()
{
    // STEP 1
    // 모든 Polygon들을 이루고 있는 모든 절점을 추출한다
    // 실제로 동일점으로 인정될 수 있는 절점들이지만 아주 미세한 오차가 나는 절점들은
    // 현 단계에서는 서로 다른점으로 인식하게 한다.
    PolygonIterator pit;
    for (pit = polygons.begin(); pit != polygons.end(); ++pit)
    {
        Polygon_2::Curve_iterator cit;
        Polygon_2& polygon = *pit;
        for (cit = polygon.curves_begin(); cit != polygon.curves_end(); ++cit)
        {
            X_monotone_curve_2& edgy = *cit;

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