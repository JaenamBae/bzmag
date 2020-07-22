#pragma once

/*
Description : Rotate Nodes
Last Update : 2016.04.20
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
//#include "expression.h"


namespace bzmag
{
namespace engine
{
    class Expression;
    class GeomRotateNode : public GeomBaseNode
    {
    public:
        GeomRotateNode();
        virtual ~GeomRotateNode();
        DECLARE_CLASS(GeomRotateNode, GeomBaseNode);

    public:
        bool setParameters(const String& angle);
        void setAngle(const String& angle);

        float64 getAngle() const;
        const String& getAngleAsString() const;

    private:
        void rotatePolygon_with_hole(const Polygon_with_holes_2& lhs, Polygon_with_holes_2& rhs);
        void rotatePolygon(const  Polygon_2& lhs, Polygon_2& rhs);
        void rotateCurves(const Curves& lhs, Curves& rhs);

        bool rotateCurve(const X_monotone_curve_2& curve, Curve_2& rotate_curve, const Transformation& transform);
        Transformation rotate_transform();

    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        Ref<Expression> angle_;
        String sangle_;
    };

#include "GeomRotateNode.inl"

}
}