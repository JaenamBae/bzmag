#include "geomclonefromnode.h"
#include "geomclonetonode.h"
#include "geomheadnode.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"


using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomCloneFromNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomCloneFromNode::GeomCloneFromNode()
{

}

//----------------------------------------------------------------------------
GeomCloneFromNode::~GeomCloneFromNode()
{
    // CloneFromNode���� CloneToNode�� Reference�� ������ �����Ƿ� 
    // CloneToNode�� �޸𸮿� ���������� ����ȴ�
    // ���� �Ʒ��ڵ�� ���� ����
    if (from_.valid()) from_->clones_.remove(this);
}

//----------------------------------------------------------------------------
void GeomCloneFromNode::setReferenceNode(GeomCloneToNode* node)
{
    if(from_.valid()) from_->clones_.remove(this);

    from_ = nullptr;
    if (node) {
        node->clones_.emplace_back(this);
        node->clones_.unique();
        from_ = node;
    }

    update();
}

//----------------------------------------------------------------------------
GeomCloneToNode* GeomCloneFromNode::getReferenceNode() const
{
    return from_;
}

//----------------------------------------------------------------------------
Polygon_set_2* GeomCloneFromNode::getPolyset()
{
    GeomBaseNode* node = from_.get<GeomBaseNode*>();
    if (node) return node->getPolyset();
    return nullptr;
}

//-----------------------------------------------------------------------------
const GeomCloneFromNode::Curves& GeomCloneFromNode::getCurves()
{
    GeomBaseNode* node = from_.get<GeomBaseNode*>();
    if (node) {
        return node->getCurves();
    }
    else {
        return curves_;
    }
}

//-----------------------------------------------------------------------------
const GeomCloneFromNode::Vertices& GeomCloneFromNode::getVertices()
{
    GeomBaseNode* node = from_.get<GeomBaseNode*>();
    if (node) {
        return node->getVertices();
    }
    else {
        return vertices_;
    }
}

//----------------------------------------------------------------------------
bool GeomCloneFromNode::hitTest(float64 x, float64 y)
{
    GeomBaseNode* node = from_.get<GeomBaseNode*>();
    if (node) return node->hitTest(x,y);
    else return false;
}

//----------------------------------------------------------------------------
bool GeomCloneFromNode::update()
{
    if (from_.invalid())
        return false;

    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
void GeomCloneFromNode::bindProperty()
{

}