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
    // ���� ������ ����
    if (geometry_) delete geometry_;
    geometry_ = 0;

    // �θ��尡 ������ ������ ������ �� ����
    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (!parent) {
        return;
    }

    Polygon_set_2* geometry = parent->getPolyset();

    // ������� geometry�� ������ ������ �� ����
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