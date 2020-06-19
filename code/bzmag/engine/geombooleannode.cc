#include "geombooleannode.h"
#include "geomheadnode.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_ABSTRACTCLASS(GeomBooleanNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomBooleanNode::GeomBooleanNode()
{

}

//----------------------------------------------------------------------------
GeomBooleanNode::~GeomBooleanNode()
{

}

//----------------------------------------------------------------------------
bool GeomBooleanNode::hitTest(float64 x, float64 y)
{
    Polygon_set_2* geometry = getPolyset();
    if (geometry) {
        Polygon_with_holes_2 poly;
        if (geometry->locate(
            Traits_2::Point_2(CoordNT(x), CoordNT(y)), poly))
            return true;
    }

    return false;
}

//----------------------------------------------------------------------------
void GeomBooleanNode::updateToolNodes()
{
    // Set new tool nodes
    toolnodes_.clear();

    Node::ConstNodeIterator it;
    for (it = firstChildNode(); it != lastChildNode(); ++it)
    {
        Node* node = *it;
        GeomHeadNode* hn = dynamic_cast<GeomHeadNode*>(node);
        if (hn) {
            hn->setStandAlone(false);
            toolnodes_.push_back(hn);
        }
    }
}

//----------------------------------------------------------------------------
bool GeomBooleanNode::update()
{
    updateToolNodes();

    // Virtual Function : Override
    boolean_operation();

    return GeomBaseNode::update();
}
