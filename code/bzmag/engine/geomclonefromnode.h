#pragma once

/*
Description : CloneFrom Node
Last Update : 2017.09.28
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geombasenode.h"


namespace bzmag
{
namespace engine
{
    class GeomCloneToNode;
    class GeomCloneFromNode : public GeomBaseNode
    {
    public:
        GeomCloneFromNode();
        virtual ~GeomCloneFromNode();
        DECLARE_CLASS(GeomCloneFromNode, GeomBaseNode);

    public:
        void setReferenceNode(GeomCloneToNode* node);
        GeomCloneToNode* getReferenceNode() const;

        // 새로운 형상 리턴 함수
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

    private:
        Ref<GeomCloneToNode> from_; // modified : 2017.09.28
    };

#include "geomclonefromnode.inl"

}
}