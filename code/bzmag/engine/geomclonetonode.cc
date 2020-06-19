#include "geomclonetonode.h"
#include "geomheadnode.h"
#include "geomclonefromnode.h"
#include "core/kernel.h"
#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomCloneToNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomCloneToNode::GeomCloneToNode()// : clone_(0)
{

}

//----------------------------------------------------------------------------
GeomCloneToNode::~GeomCloneToNode()
{
    clones_.clear();
}

//----------------------------------------------------------------------------
GeomCloneToNode::FromIterator GeomCloneToNode::firstClonedNode()
{
    return clones_.begin();
}

//----------------------------------------------------------------------------
GeomCloneToNode::FromIterator GeomCloneToNode::lastClonedNode()
{
    return clones_.end();
}

//----------------------------------------------------------------------------
Polygon_set_2* GeomCloneToNode::getPolyset()
{
    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (parent) return parent->getPolyset();
    return nullptr;
}

//-----------------------------------------------------------------------------
const GeomCloneToNode::Curves& GeomCloneToNode::getCurves()
{
    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (parent) {
        return parent->getCurves();
    }
    else {
        return curves_;
    }
}

//-----------------------------------------------------------------------------
const GeomCloneToNode::Vertices& GeomCloneToNode::getVertices()
{
    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (parent) {
        return parent->getVertices();
    }
    else {
        return vertices_;
    }
}

//----------------------------------------------------------------------------
bool GeomCloneToNode::hitTest(float64 x, float64 y)
{
    GeomBaseNode* node = dynamic_cast<GeomBaseNode*>(getParent());
    if (node) return node->hitTest(x,y);
    else return false;
}

//----------------------------------------------------------------------------
bool GeomCloneToNode::update()
{
//    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    FromIterator it;
    for (it = clones_.begin(); it != clones_.end(); ++it)
    {
        GeomCloneFromNode* fromNode = *it;
        fromNode->update();
    }

    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
void GeomCloneToNode::bindProperty()
{

}
