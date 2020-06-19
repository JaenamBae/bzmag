#pragma once

/*
Description : Vertex Object
Last Update : 2019.04.24
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "core/object.h"
#include "core/ref.h"

namespace bzmag
{
namespace engine
{
    class Expression;

    // cs_ 참조기능, hitTest() 메써드 등을 활용하기 위해
    // GeomBasedNode를 상속함
    class GeomVertex : public Object
    {
    public:
        GeomVertex();
        virtual ~GeomVertex();
        DECLARE_CLASS(GeomVertex, Object);

    public:
        bool setParameters(const String& x, const String& y);
        void setX(const String& x);
        void setY(const String& y);
        float64 getX() const;
        float64 getY() const;

        const String& getXasString() const;
        const String& getYasString() const;

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        Ref<Expression> x_;
        Ref<Expression> y_;

        String sx_;
        String sy_;
    };

#include "geomvertex.inl"

}
}