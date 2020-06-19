#pragma once

/*
Description : CoverLine Node
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"

namespace bzmag
{
namespace engine
{
    class GeomCoverLineNode : public GeomBaseNode
    {
    public:
        GeomCoverLineNode();
        virtual ~GeomCoverLineNode();
        DECLARE_CLASS(GeomCoverLineNode, GeomBaseNode);

    public:
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();


    private:
        bool check_closed(const Curves& curves);
        X_monotone_curve_2 regenerate_curve(const X_monotone_curve_2& curve);

    public:
        static void bindMethod();
        static void bindProperty();
    };

#include "geomcoverlinenode.inl"

}
}