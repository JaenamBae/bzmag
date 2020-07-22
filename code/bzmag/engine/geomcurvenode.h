#pragma once

/*
Description : Curve Node
Last Update : 2019.05.11
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geometric_entity.h"
#include "geombasenode.h"
#include "core/ref.h"
#include "core/vector2.h"

namespace bzmag
{
namespace engine
{
    class Expression;
    class GeomCurveNode : public GeomBaseNode
    {
    public:
        GeomCurveNode();
        virtual ~GeomCurveNode();
        DECLARE_CLASS(GeomCurveNode, GeomBaseNode);

    public:
        bool setParameters(const String& start, const String& end, const String& center = "");

        void setStartPoint(const String& start);
        void setEndPoint(const String& end);
        void setMidPoint(const String& center);

        const String& getStartPoint() const;
        const String& getEndPoint() const;
        const String& getMidPoint() const;
        bool pointOnCurve(float64 x, float64 y) const;

    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();



    private:
        Ref<Expression> sx_;
        Ref<Expression> sy_;
        Ref<Expression> ex_;
        Ref<Expression> ey_;
        Ref<Expression> mx_;
        Ref<Expression> my_;

        // 시작점
        String sstart_;

        // 끝점
        String send_;

        // 중앙점
        String smid_;

    private:
        static float64 torr_;
    };

#include "geomcurvenode.inl"

}
}