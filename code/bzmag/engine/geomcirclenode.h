#pragma once

/*
Description : Circle Node
Last Update : 2017.09.28
- Big change of Expression
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
#include "core/vector2.h"
#include "core/ref.h"

namespace bzmag
{
namespace engine
{
    class Expression;
    class GeomCircleNode : public GeomBaseNode
    {
    public:
        GeomCircleNode();
        virtual ~GeomCircleNode();
        DECLARE_CLASS(GeomCircleNode, GeomBaseNode);

    public:
        bool setParameters(const String& center, const String& radius);
        void setCenter(const String& c);
        void setRadius(const String& radius);

        const String& getCenter() const;
        const String& getRadius() const;


    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        Ref<Expression> cx_;
        Ref<Expression> cy_;
        Ref<Expression> radii_;

        String scenter_;
        String sradii_;
    };

#include "geomcirclenode.inl"

}
}