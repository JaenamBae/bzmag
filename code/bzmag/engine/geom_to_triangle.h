#pragma once

/*
Description : Convert Geometries to Mesh Generator (Triangle) Input File Format (*.POLY)
Last Update : 2016.05.25
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/node.h"
#include "core/singleton3.h"
#include "core/vector2.h"
#include "geomheadnode.h"

namespace bzmag
{
namespace engine
{
    class GeomToPolyTriangle : public Node, public Singleton3<GeomToPolyTriangle>
    {
    public:
        typedef std::list<Polygon_2> PolygonList;
        typedef PolygonList::const_iterator PolygonIterator;
        typedef std::list<Traits_2::Point_2> PointList;
        typedef PointList::const_iterator PointIterator;
        typedef std::list<X_monotone_curve_2> EdgyList;

        // <x> <y> [attributes] [boundary marker]
        typedef std::map<uint32, Vector2> Vertices;
        typedef std::map<uint32, uint32> VertexMarkers;
        

        // <endpoint> <endpoint> [boundary marker]
        typedef std::map<uint32, Tuple2<uint32>> Segments;
        typedef std::map<uint32, uint32> SegmentMarkers;

        // <x> <y>
        typedef std::map<uint32, Vector2> Holes;

        // <x> <y> <attribute> <maximum area>
        typedef std::map<uint32, Tuple2<Vector2>> Regions;

    public:
        GeomToPolyTriangle();
        virtual ~GeomToPolyTriangle();

        DECLARE_CLASS(GeomToPolyTriangle, Node);

    public:
        void convert(const String& path);
        Vertices::const_iterator firstVertex() const;
        Vertices::const_iterator lastVertex() const;
        Segments::const_iterator firstSegment() const;
        Segments::const_iterator lastSegment() const;
        Regions::const_iterator firstRegion() const;
        Regions::const_iterator lastRegion() const;


    public:
        static void bindMethod();
        static void bindProperty();

    protected:
        bool convertPolyStructure(const Node* node);
        void extractPolygons(const Node* poly_root, PolygonList& polygons);
        void extractPoints(const PolygonList& polygons, PointList& points);
        void spiltPolygon(const Polygon_2& polygon, const PointList& points, Polygon_2& spilted_polygon);
        Polygon_2 approximatePolygon(const Polygon_2& polygon, float64 base_length);
        void removeDuplicate(PolygonList& polygons);
        void makeTriangleInput(const PolygonList& polygons);

    private:
        void spilt_edgy(const X_monotone_curve_2& curve, const PointList& points, EdgyList& spilted_edgies);
        bool point_on_curve(const X_monotone_curve_2& curve, const Traits_2::Point_2& pt);
        float64 approximate_double(float64 val);

        static bool compare_points(const Traits_2::Point_2& first, const Traits_2::Point_2& second);
        static bool check_inside(Point_2 pt, Point_2 *pgn_begin, Point_2 *pgn_end, CGALKernel traits);

    private:
        Vertices vertices_;
        VertexMarkers vertex_markers_;
        Segments segments_;
        SegmentMarkers segment_markers_;
        Holes holes_;
        Regions regions_;

    private:
        static X_monotone_curve_2* current_edgy_;
        static float64 torr_;
    };
}
}