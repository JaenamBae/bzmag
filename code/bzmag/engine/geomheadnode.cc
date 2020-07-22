#include "geomheadnode.h"
#include "geombooleannode.h"

#include "CSnode.h"
#include "BCnode.h"
#include "materialnode.h"

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

	// �ص��尡 �����ϴ� ������ �����带 �������
	// ������ ����� ���� �ٿ�帮 ��ü(��ƼƼ)�� �籸�� �Ѵ�.
	//updateEntity();

    // head�� �θ� (head����� �θ�� boolean node�� �� �ִ�)�� ������Ʈ �Ѵ�. 
    // ���ÿ� �θ��� �ڽĳ�� �� GeomHeadNode�� ������
    // GeomNode�� ����� �ڽ� ������ ������Ʈ �ȴ�
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
bool GeomHeadNode::applyBC(uint64 index, BCNode* bc)
{
	BCs::iterator it = BCs_.find(index);
	if (curves_.size() >= index) {
		BCs_[index] = bc;
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------
Polygon_set_2* GeomHeadNode::getPolyset()
{
    if (lastNode_.valid()) {
        return lastNode_->getPolyset();
    }
    else {
        return nullptr;
    }
}

//-----------------------------------------------------------------------------
const GeomHeadNode::Curves& GeomHeadNode::getCurves()
{
    if (lastNode_.valid()) {
        return lastNode_->getCurves();
    }
    else {
        return curves_;
    }
}

//-----------------------------------------------------------------------------
const GeomHeadNode::Vertices& GeomHeadNode::getVertices()
{
    if (lastNode_.valid()) {
        return lastNode_->getVertices();
    }
    else {
        return vertices_;
    }
}

//----------------------------------------------------------------------------
bool GeomHeadNode::hitTest(float64 x, float64 y)
{
    if (lastNode_.valid()) {
        return lastNode_->hitTest(x, y);
    }
    else {
        return false;
    }
}

//----------------------------------------------------------------------------
bool GeomHeadNode::update()
{
    // �ص��� ������Ʈ �ô� �ƹ����� ���Ѵ�
    // ����) GeomBaseNode ������Ʈ �ô� �ڽĳ����� ��� ������Ʈ �Ѵ�
    //       �ص��带 ������ GeomBaseNode ��� ���� �θ����� ������ �ޱ� �����̴�
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

