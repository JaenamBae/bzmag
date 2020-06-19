#include "geomheadnode.h"
#include "geombooleannode.h"
#include "csnode.h"
#include "geomvertex.h"
#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomHeadNode, GeomBaseNode);

//----------------------------------------------------------------------------
float64 GeomHeadNode::tolerance_ = 1;

//----------------------------------------------------------------------------
GeomHeadNode::GeomHeadNode() :
    bStandalone_(true),
    bModelNode_(true),
    color_(230, 230, 230, 200),
    hide_(false),
    lastNode_(nullptr),
    num_elements_(0)
{
    bHead_ = true;
}

//----------------------------------------------------------------------------
GeomHeadNode::~GeomHeadNode()
{

}

//----------------------------------------------------------------------------
void GeomHeadNode::setLastNode(GeomBaseNode* last)
{
    lastNode_ = last;

    // head의 부모 (head노드의 부모는 boolean node일 수 있다)를 업데이트 한다. 
    // 동시에 부모의 자식노드 중 GeomHeadNode를 제외한
    // GeomNode를 상속한 자식 노드들이 업데이트 된다
    GeomBaseNode* parent = dynamic_cast<GeomBaseNode*>(getParent());
    if (parent) parent->update();
}

//----------------------------------------------------------------------------
void GeomHeadNode::setMaterialNode(Node* material)
{
    material_ = material;
}

//----------------------------------------------------------------------------
Node* GeomHeadNode::getMaterialNode() const
{
    return material_;
}

//----------------------------------------------------------------------------
Polygon_set_2* GeomHeadNode::getPolyset()
{
    if (lastNode_) {
        return lastNode_->getPolyset();
    }
    else {
        return nullptr;
    }
}

//-----------------------------------------------------------------------------
const GeomHeadNode::Curves& GeomHeadNode::getCurves()
{
    if (lastNode_) {
        return lastNode_->getCurves();
    }
    else {
        return curves_;
    }
}

//-----------------------------------------------------------------------------
const GeomHeadNode::Vertices& GeomHeadNode::getVertices()
{
    if (lastNode_) {
        return lastNode_->getVertices();
    }
    else {
        return vertices_;
    }
}

//----------------------------------------------------------------------------
bool GeomHeadNode::hitTest(float64 x, float64 y)
{
    if (lastNode_) {
        return lastNode_->hitTest(x, y);
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
bool GeomHeadNode::update()
{
    // 해드노드 업데이트 시는 아무짓도 안한다
    // 참고) GeomBaseNode 업데이트 시는 자식노드들을 모두 업데이트 한다
    //       해드노드를 제외한 GeomBaseNode 상속 노드는 부모노드의 영향을 받기 때문이다
    return true;
}

//----------------------------------------------------------------------------
void GeomHeadNode::onAttachTo(Node* parent)
{
    GeomBaseNode* node = dynamic_cast<GeomBaseNode*>(parent);
    if(node) node->update();
}


//----------------------------------------------------------------------------
void GeomHeadNode::onDetachFrom(Node* parent)
{
    GeomBaseNode* node = dynamic_cast<GeomBaseNode*>(parent);
    if (node) node->update();
}


//----------------------------------------------------------------------------
void GeomHeadNode::bindProperty()
{
    BIND_PROPERTY(const Color&, Color, &setColor, &getColor);
    BIND_PROPERTY(bool, IsHide, &setHideStatus, &isHide);
    BIND_PROPERTY(bool, IsModel, &setModelNode, &isModelNode);
    BIND_PROPERTY(bool, IsStandAlone, 0, &isStandAlone);
    BIND_PROPERTY(Node*, Material, &setMaterialNode, &getMaterialNode);
    BIND_PROPERTY(int32, RequiredNumberOfElements, &setNumberOfElements, &getNumberOfElements);
}

