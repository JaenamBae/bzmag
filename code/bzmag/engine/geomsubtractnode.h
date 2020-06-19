#pragma once

/*
Description : Abstract Node for Boolean Operation
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombooleannode.h"


namespace bzmag
{
namespace engine
{
    class GeomSubtractNode : public GeomBooleanNode
    {
    public:
        GeomSubtractNode();
        virtual ~GeomSubtractNode();
        DECLARE_CLASS(GeomSubtractNode, GeomBooleanNode);
        
    public:
        virtual String description() const;

    public:
        static void bindMethod();
        static void bindProperty();

    protected:
        virtual void boolean_operation();
    };

#include "geomsubtractnode.inl"

}
}