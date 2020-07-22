#pragma once

/*
Description : Move Nodes
Comment : Move 노드는 Move를 위한 기준좌표계가 주어져야함
          기준좌표계는 GeomBaseNode에 구현된 setReferedCS()함수를 통해 설정할 수 있음
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
//#include "expression.h"


namespace bzmag
{
namespace engine
{
    class Expression;
    class GeomMoveNode : public GeomBaseNode
    {
    public:
        GeomMoveNode();
        virtual ~GeomMoveNode();
        DECLARE_CLASS(GeomMoveNode, GeomBaseNode);

    public:
        bool setParameters(const String& dx, const String& dy);
        void set_dx(const String& dx);
        void set_dy(const String& dx);

        float64 get_dx() const;
        float64 get_dy() const;
        const String& get_dx_asString() const;
        const String& get_dy_asString() const;

    private:
        void movePolygon_with_hole(const Polygon_with_holes_2& lhs, Polygon_with_holes_2& rhs);
        void movePolygon(const Polygon_2& lhs, Polygon_2& rhs);
        void moveCurves(const Curves& lhs, Curves& rhs);

        bool moveCurve(const X_monotone_curve_2& curve, Curve_2& rotate_curve, const Vector_2& disp);
        Vector_2 displacement();

    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();

    private:


    private:
        Ref<Expression> dx_;
        Ref<Expression> dy_;

        String sdx_;
        String sdy_;
    };

#include "GeomMoveNode.inl"

}
}