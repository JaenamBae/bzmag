#pragma once

/*
Description : Band Node
Last Update : 2018.06.14
- Big change of Expression
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geomsurfacenode.h"
#include "core/vector2.h"
#include "core/ref.h"

namespace bzmag
{
namespace engine
{
    class Expression;
    class GeomBandNode : public GeomSurfaceNode
    {
    public:
        GeomBandNode();
        virtual ~GeomBandNode();
        DECLARE_CLASS(GeomBandNode, GeomSurfaceNode);

    public:
        bool setParameters(const String& x, const String& y, const String& radius, const String& width);
        void setCenterX(const String& x);
        void setCenterY(const String& y);
        void setRadius(const String& radius);
        void setWidth(const String& width);

        Vector2 getCenter() const;
        float64 getCenterX() const;
        float64 getCenterY() const;
        float64 getRadius() const;
        float64 getWidth() const;

        const String& getCenterXasString() const;
        const String& getCenterYasString() const;
        const String& getRadiusasString() const;
        const String& getWidthasString() const;


    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        static void bindMethod();
        static void bindProperty();

    public:
        virtual bool cover();

    private:
        Ref<Expression> cx_;
        Ref<Expression> cy_;
        Ref<Expression> radii_;
        Ref<Expression> width_;

        String scx_;
        String scy_;
        String sradii_;
        String swidth_;
    };

#include "geombandnode.inl"

}
}