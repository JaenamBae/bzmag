//-----------------------------------------------------------------------------
inline bool operator == (const Node::RefNode& s1, const String& s2)
{
    Node* n = s1;
    return n->getName() == s2;
}

//-----------------------------------------------------------------------------
inline const String& Node::getName() const
{
    return name_;
}


//-----------------------------------------------------------------------------
inline Node* Node::getParent()
{
    return parent_;
}


//-----------------------------------------------------------------------------
template <typename DERIVED>
void Node::clearChildren()
{
    for (NodeIterator iter = firstChildNode();
        iter != lastChildNode();)
    {
        DERIVED* o = *iter;
        if (o)
        {
            iter = o->detach();
            continue;
        }
        ++iter;
    }
}


//-----------------------------------------------------------------------------
inline size_t Node::getNumChildren() const
{
    return children_.size();
}


//-----------------------------------------------------------------------------
inline Node::NodeIterator Node::firstChildNode()
{
    return children_.begin();
}


//-----------------------------------------------------------------------------
inline Node::NodeIterator Node::lastChildNode()
{
    return children_.end();
}


//-----------------------------------------------------------------------------
inline Node::ConstNodeIterator Node::firstChildNode() const
{
    return children_.begin();
}


//-----------------------------------------------------------------------------
inline Node::ConstNodeIterator Node::lastChildNode() const
{
    return children_.end();
}
