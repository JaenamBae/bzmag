#pragma once

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>


namespace bzmag
{
namespace engine
{
    typedef CGAL::Exact_predicates_exact_constructions_kernel CGALKernel;
    typedef CGAL::Aff_transformation_2<CGALKernel>            Transformation;
    typedef CGAL::Vector_2<CGALKernel>                        Vector_2;
    typedef CGAL::Gps_circle_segment_traits_2<CGALKernel>     Traits_2;
    typedef CGAL::General_polygon_set_2<Traits_2>             Polygon_set_2;

    typedef CGALKernel::FT                                    NT;
    typedef CGALKernel::Point_2                               Point_2;
    typedef CGALKernel::Line_2                                Line_2;
    typedef CGALKernel::Circle_2                              Circle_2;
    typedef CGALKernel::Iso_rectangle_2                       Rectangle_2;

    typedef Traits_2::General_polygon_2                       Polygon_2;
    typedef Traits_2::General_polygon_with_holes_2            Polygon_with_holes_2;
    typedef Traits_2::Curve_2                                 Curve_2;
    typedef Traits_2::X_monotone_curve_2                      X_monotone_curve_2;
    typedef Traits_2::CoordNT                                 CoordNT;
}
}