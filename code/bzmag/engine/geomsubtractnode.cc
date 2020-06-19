#include "geomsubtractnode.h"
#include "geomheadnode.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;


IMPLEMENT_CLASS(GeomSubtractNode, GeomBooleanNode);

//----------------------------------------------------------------------------
GeomSubtractNode::GeomSubtractNode()
{
    
}

//----------------------------------------------------------------------------
GeomSubtractNode::~GeomSubtractNode()
{
    
}

//----------------------------------------------------------------------------
void GeomSubtractNode::boolean_operation()
{
    // 기존 데이터 삭제
    if (geometry_) delete geometry_;
    geometry_ = 0;

    // 부모노드가 없으면 연산을 수행할 수 없음
    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (!parent) {
        return;
    }

    Polygon_set_2* geometry = parent->getPolyset();

    // 만들어진 geometry가 없으면 연산할 수 없음
    if (nullptr == geometry) return;

    Polygon_set_2* op = new Polygon_set_2(*geometry);
    ToolIter it;
    for (it = toolnodes_.begin(); it != toolnodes_.end(); ++it)
    {
        GeomHeadNode* tool = *it;
        Polygon_set_2* geom_tool = tool->getPolyset();
        if (geom_tool) {
            op->difference(*geom_tool);
        }
    }
    op->remove_redundant_edges();
    geometry_ = op;
}

//----------------------------------------------------------------------------
void GeomSubtractNode::bindProperty()
{
    
}