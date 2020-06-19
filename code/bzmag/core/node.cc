#include "node.h"

#include "simplepropertybinder.h"
#include "nodeeventpublisher.h"

using namespace bzmag;

IMPLEMENT_CLASS(Node, Object);

//-----------------------------------------------------------------------------
Node::Node() : parent_(0)
{
    NodeEventPublisher::instance()->addRefSingleton();
}


//-----------------------------------------------------------------------------
Node::~Node()
{
    // clear children
    for (NodeIterator iter = firstChildNode();
        iter != lastChildNode(); ++iter)
    {
        Node* node = *iter;
        node->parent_ = 0;
    }
    children_.clear();

    if (parent_)
    {
        parent_->onRemoveNode(this);
        onDetachFrom(parent_);
        NodeEventPublisher::instance()->onDetachFrom(parent_, this);
    }
    parent_ = 0;


    NodeEventPublisher::instance()->releaseSingleton();
}


//-----------------------------------------------------------------------------
// Modified : 2017.09.27 ; detach�� ��ġ�ʴ� �޸� ������ �� �� �־�
//            RefCount�� �������״ٰ� �۾��� �� ���� ������ �ٽ� ���ҽ�Ŵ
void Node::setName(const String& name)
{
    if (parent_)
    {
        // check node name where parent children nodes
        Node* node = parent_->findChild(name);
        if (node)
            return;

        Node* parent = parent_;
        addRef();
        detach();
        name_ = name;
        parent->attach(this);
        release();
    }
    else
        name_ = name;

    onSetName(name);
}


//-----------------------------------------------------------------------------
Path Node::getAbsolutePath()
{
    std::stack<Node*> s;
    Node* cur = this;
    while (cur)
    {
        if (cur->parent_)
            s.push(cur);
        cur = cur->parent_;
    }

    String path;
    while (!s.empty())
    {
        path += "/";
        path += s.top()->getName();
        s.pop();
    }

    return path;
}


//-----------------------------------------------------------------------------
// Modified : 2017.09.27 ; attach ������ detach ���� ��Ŵ
void Node::attach(Node* child)
{
    // ���� �ڽĳ�带 �켱 ���� �θ𿡼� ��� ����
    // �� detach�� �ϸ� RefCount �� ������ �޸� ������ �� �� �����Ƿ�
    // RefCount�� ������Ų�� detach�� �ϰ� 
    // �ٽ� RefCount�� ���ҽ�Ų��
    child->addRef();
    child->detach();
    children_.push_back(Nodes::value_type(child));
    child->release();
    child->parent_ = this;

    onAddNode(child);
    child->onAttachTo(this);
    NodeEventPublisher::instance()->onAttachTo(this, child);
}


//-----------------------------------------------------------------------------
Node::NodeIterator Node::detach()
{
    Node* parent = parent_;
    parent_ = 0;
    if (parent)
    {
        NodeIterator fi = std::find(parent->children_.begin(),
            parent->children_.end(), getName());

        if (parent->children_.end() != fi)
        {
            NodeEventPublisher::instance()->onDetachFrom(parent, this);
            this->onDetachFrom(parent);
            parent->onRemoveNode(this);
            return parent->children_.erase(fi);
        }
    }
    return lastChildNode();
}

//-----------------------------------------------------------------------------
Node* Node::findChild(const String& name)
{
    NodeIterator iter = std::find(children_.begin(),
        children_.end(), name);

    if (children_.end() == iter)
        return 0;
    return *iter;
}

//-----------------------------------------------------------------------------
Node* Node::findPrevChild(const String& name)
{
    NodeIterator iter = std::find(children_.begin(),
        children_.end(), name);

    if (children_.end() == iter || children_.begin() == iter)
        return 0;

    return *(--iter);
}

//-----------------------------------------------------------------------------
Node* Node::findNextChild(const String& name)
{
    NodeIterator iter = std::find(children_.begin(),
        children_.end(), name);

    if (children_.end() == iter || children_.end() == ++iter)
        return 0;

    return *iter;
}

//-----------------------------------------------------------------------------
Node* Node::relativeNode(const Path& path)
{
    if (path == ".")
        return this;
    else if (path == "..")
        return getParent();
    else
    {
        Node* cur = this;
        for (Path::const_iterator token = path.begin(); token != path.end(); ++token)
        {
            Node* child = cur->findChild(*token);
            if (0 == child)
                return 0;
            cur = child;
        }
        return cur;
    }
}


//-----------------------------------------------------------------------------
void Node::onSetName(const String& name)
{
    // empty
}


//-----------------------------------------------------------------------------
void Node::onAttachTo(Node* parent)
{
    // empty
}


//-----------------------------------------------------------------------------
void Node::onDetachFrom(Node* parent)
{
    // empty
}

//-----------------------------------------------------------------------------
void Node::onAddNode(Node* node)
{
    // empty
}


//-----------------------------------------------------------------------------
void Node::onRemoveNode(Node* node)
{
    // empty
}


//-----------------------------------------------------------------------------
bool Node::update()
{
    // empty
    return true;
}


//-----------------------------------------------------------------------------
void Node::bindProperty()
{
    BIND_PROPERTY(size_t, num_children, 0, &getNumChildren);
    BIND_PROPERTY(const String&, name, &setName, &getName);
}
