#pragma once

/*
Description : Convert Geometries to Mesh Generator (Triangle) Input File Format (*.POLY)
Last Update : 2016.05.25
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/node.h"
#include "core/singleton3.h"
#include "geomheadnode.h"

namespace bzmag
{
namespace engine
{
    class GeometryConverter : public Node, public Singleton3<GeometryConverter>
    {
        typedef std::list<Polygon_2> PolygonList;
        typedef PolygonList::iterator PolygonIterator;

        typedef std::list<Traits_2::Point_2> PointList;
        typedef PointList::iterator PointIterator;

        typedef std::list<X_monotone_curve_2> EdgyList;

    public:
        GeometryConverter();
        virtual ~GeometryConverter();

        DECLARE_CLASS(GeometryConverter, Node);

        bool convert(const String& path);


    private:
        bool extract_base_points();


    private:
        PointList point_list_;
        EdgyList  edgy_list_;
        PolygonList polygon_list_;
    };
}
}