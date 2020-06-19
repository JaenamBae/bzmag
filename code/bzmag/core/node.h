#ifndef BZMAG_CORE_OBJECT_NODE_H
#define BZMAG_CORE_OBJECT_NODE_H
/**
@ingroup bzmagCoreObject
@class bzmag::Node
@brief
*/

#include <vector>
#include <stack>
#include "type.h"
#include "object.h"
#include "path.h"
#include "ref.h"

namespace bzmag
{
    class Node : public Object
    {
    public:
        DECLARE_CLASS(Node, Object);

    public:
        typedef Ref<Node> RefNode;
        typedef std::list<RefNode> Nodes;       // vector�� ����� ����....
        typedef Nodes::iterator NodeIterator;
        typedef Nodes::const_iterator ConstNodeIterator;
        friend bool operator == (const RefNode& s1, const String& s2);

    public:
        Node();
        virtual~Node();
        bool operator==(const String& name)
        {
            return name_ == name;
        };

        void setName(const String& name);
        const String& getName() const;
        Path getAbsolutePath();

        Node* getParent();
        void attach(Node* child);
        NodeIterator detach();
        template <typename DERIVED>
        void clearChildren();
        Node* findChild(const String& name);
        Node* findPrevChild(const String& name);
        Node* findNextChild(const String& name);
        Node* relativeNode(const Path& path);

        size_t getNumChildren() const;
        NodeIterator firstChildNode();
        NodeIterator lastChildNode();
        ConstNodeIterator firstChildNode() const;
        ConstNodeIterator lastChildNode() const;

        virtual void onSetName(const String& name);
        virtual void onAttachTo(Node* parent);
        virtual void onDetachFrom(Node* parent);
        virtual void onAddNode(Node* node);
        virtual void onRemoveNode(Node* node);

        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();

    private:
        Node* parent_;
        String name_;
        Nodes children_;

//     private:
//         friend class Kernel;
    };



#include "node.inl"

}

#endif // BZMAG_CORE_OBJECT_NODE_H
