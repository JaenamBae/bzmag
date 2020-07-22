#pragma once

/*
Description : CloneTo Node
Last Update : 2016.04.23
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"


namespace bzmag
{
namespace engine
{
    class GeomCloneFromNode;
    class GeomCloneToNode : public GeomBaseNode
    {
        friend class GeomCloneFromNode;

    public:
        // how to change the list to set? (duplicate problem)
        typedef std::list<Ref<GeomCloneFromNode>> FromNodes;
        typedef FromNodes::iterator FromIterator;

        GeomCloneToNode();
        virtual ~GeomCloneToNode();
        DECLARE_CLASS(GeomCloneToNode, GeomBaseNode);

    public:
        FromIterator firstClonedNode();
        FromIterator lastClonedNode();

    public:
        virtual Polygon_set_2* getPolyset();
        virtual const Curves& getCurves();
        virtual const Vertices& getVertices();

        virtual bool hitTest(float64 x, float64 y);
        virtual String description() const;

    public:
        virtual bool update();

    public:
        static void bindMethod();
        static void bindProperty();


    protected:
        // GeomCloneFrom 노드에서만 설정할 수 있게 바꾸었음
        FromNodes clones_;
    };

#include "geomclonetonode.inl"

}
}