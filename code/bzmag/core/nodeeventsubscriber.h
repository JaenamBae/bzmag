#ifndef BZMAG_CORE_OBJECT_NODEEVENTSUBSCRIBER_H
#define BZMAG_CORE_OBJECT_NODEEVENTSUBSCRIBER_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::NodeEventSubscriber
    @brief
*/

namespace bzmag
{
    class Node;
    class NodeEventSubscriber
    {
    public:
        virtual void onAttachTo(Node* parent, Node* child) {}
        virtual void onDetachFrom(Node* parent, Node* child) {}
    };
}

#endif // BZMAG_CORE_OBJECT_NODEEVENTSUBSCRIBER_H
