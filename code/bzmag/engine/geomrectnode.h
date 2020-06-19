#pragma once

/*
Description : Rectangular Node
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
#include "core/vector2.h"


namespace bzmag
{
namespace engine
{
    class Expression;
    class GeomRectNode : public GeomBaseNode
    {
    public:
        GeomRectNode();
        virtual ~GeomRectNode();
        DECLARE_CLASS(GeomRectNode, GeomBaseNode);

    public:
        bool setParameters(const String& point, 
            const String& dx, 
            const String& dy);

        void setPoint(const String& point);
        void setWidth(const String& width);
        void setHeight(const String& height);

        const String& getPoint() const;
        const String& getWidth() const;
        const String& getHeight() const;

    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        Ref<Expression> px_;
        Ref<Expression> py_;
        Ref<Expression> width_;
        Ref<Expression> height_;

        String spoint_;
        String swidth_;
        String sheight_;
    };

#include "GeomRectNode.inl"

}
}