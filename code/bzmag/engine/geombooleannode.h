#pragma once

/*
Description : Abstract Node for Boolean Operation
Last Update : 2017.09.29
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"
#include <list>


namespace bzmag
{
namespace engine
{
    class GeomBooleanNode : public GeomBaseNode
    {
    public:
        typedef std::list<GeomHeadNode*> ToolNodes;
        typedef ToolNodes::iterator ToolIter;

    public:
        GeomBooleanNode();
        virtual ~GeomBooleanNode();
        DECLARE_ABSTRACTCLASS(GeomBooleanNode, GeomBaseNode);
        
    protected:
        ToolIter firstToolNode()
        { return toolnodes_.begin(); }

        ToolIter lastToolNode()
        { return toolnodes_.end(); }

        void updateToolNodes();
        virtual void boolean_operation() = 0;


    public:
        //virtual Polygon_set_2* getGeometry();
        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const = 0;

    public:
        virtual bool update();


    protected:
        ToolNodes toolnodes_;
    };
}
}