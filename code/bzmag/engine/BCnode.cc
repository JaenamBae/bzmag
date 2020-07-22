#include "BCnode.h"
#include "expression.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(BCNode, Node);

//----------------------------------------------------------------------------
BCNode::BCNode() : type_(UNDEFINED)
{
	uint32 key = getID();
	value_ = new Expression();
	value_->setKey("bc_" + std::to_string(key));
}

//----------------------------------------------------------------------------
BCNode::~BCNode()
{

}

//----------------------------------------------------------------------------
bool BCNode::setBCValue(const String& value)
{
	if (!value_->setExpression(value))
		return false;
	return true;
}

//----------------------------------------------------------------------------
bool BCNode::update()
{
    return true;
}

//----------------------------------------------------------------------------
void BCNode::onAttachTo(Node* parent)
{

}


//----------------------------------------------------------------------------
void BCNode::onDetachFrom(Node* parent)
{

}

//----------------------------------------------------------------------------
void BCNode::bindProperty()
{
	BIND_PROPERTY(BCTYPE, BC_Type, &setBCType, &getBCType);
}

