#pragma once

/*
Description : Convert Geometries to Mesh Generator (Triangle) Input File Format (*.POLY)
Last Update : 2016.05.25
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/node.h"
#include "core/singleton3.h"
#include "core/vector2.h"
#include "polyholeshelper.h"

namespace bzmag
{
namespace engine
{
    class GeomToPolyTriangle : public Node, public Singleton3<GeomToPolyTriangle>
    {
    public:
        struct Point_2_sort
        {
            Point_2_sort() {};
            ~Point_2_sort() {};
            bool operator() (const Traits_2::Point_2& lhs, const Traits_2::Point_2& rhs) const;
        };

        struct OnPoint_2_sort
        {
            OnPoint_2_sort(const X_monotone_curve_2& edge):current_edge_(edge) {};
            ~OnPoint_2_sort() {};
            const X_monotone_curve_2& current_edge_;
            bool operator() (const Traits_2::Point_2& lhs, const Traits_2::Point_2& rhs) const;
        };

        typedef std::list<Traits_2::Point_2> PointList;
        typedef std::list<X_monotone_curve_2> EdgeList;
        typedef std::vector<PolyHolesHelper> PolyHolesList;



    public:
        GeomToPolyTriangle();
        virtual ~GeomToPolyTriangle();
        DECLARE_CLASS(GeomToPolyTriangle, Node);

    public:
        bool makePolyStructures(const String& path);
        PolyHolesList::const_iterator firstPolyHoles() const;
        PolyHolesList::const_iterator lastPolyHoles() const;
        const PolyHolesHelper* getPolyHoles(uint32 index);
        uint64 getNumberOfPolyHoles() const;
        void getPointsOnHoles(std::list<Vector2>& result);
        //std::list<Vector2>::const_iterator firstAssistedPoint() const;
        //std::list<Vector2>::const_iterator lastAssistedPoint() const;
        bool hitTest(const Vector2& pt);

        float64 getDomainArea() const;

    private:
        void makePolyHoles(const Node* poly_root);
        bool checkPolyHoles();
        void spiltupPolyHoles();
        void extractBasedPointsFromPolyHoles();
        void extractBasedPointsFromPolygon(const Polygon_2& polygon);
        
        Polygon_with_holes_2 regeneratePolyHolesWithBasedPoints(const Polygon_with_holes_2& polyholes) const;
        Polygon_2 regeneratePolygonWithBasedPoints(const Polygon_2& polygon) const;
        void getBasedPointsOnCurve(const X_monotone_curve_2& curve, PointList& onpoints) const;
        bool pointOnCurve(const X_monotone_curve_2& curve, const Traits_2::Point_2& point) const;
        void spiltEdgeByPoints(const X_monotone_curve_2& curve, const PointList& points, EdgeList& output) const;

        Polygon_with_holes_2 approximatePolyHoles(const Polygon_with_holes_2& polyholes, float64 base_length) const;
        Polygon_2 approximatePolygon(const Polygon_2& polygon, float64 base_length) const;
        float64 approximate_double(float64 val) const;

        void makeTriangleInput(PolyHolesHelper& polyholes) const;
        bool getRandomPointInPolygon(const Polygon_2& polygon, Vector2& pt) const;
        
        float64 calculateBasedSegmentLength() const;

        void setupMeshConditions();

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        PointList based_points_;
        //std::list<Vector2> assisted_points_;
        PolyHolesList based_polyholes_;

    private:
        static float64 torr_;

        // 弥家 夸家 积己 肮荐?
        static uint32 based_elements_num_;
    };
}
}