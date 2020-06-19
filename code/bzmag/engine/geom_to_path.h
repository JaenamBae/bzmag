#pragma once

/*
Description : Geometry Node to vertex list Converter (helper class)
Last Update : 2018.11.07
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

//#include "agg/agg_path_storage.h"
#include "geometric_entity.h"
#include "geomheadnode.h"
#include "core/primitivetype.h"
#include "core/vector2.h"

//using namespace agg;

namespace bzmag
{
namespace engine
{
    class GeomToPath
    {
    public:
        struct VertexInfo {float64 x; float64 y; uint32 cmd;};
        typedef std::list<VertexInfo> VertexList;

        GeomToPath(GeomHeadNode* render_node);
        virtual ~GeomToPath();

    public:
        void setNormalDeviation(uint32 num);
        uint32 getNormalDeviation() const;
        bool makePath(VertexList& path);

    protected:
        void makeBoundary(Polygon_2& p, VertexList& path);
        void makeBoundary(const GeomBaseNode::Curves& c, VertexList& path);
        void makeBoundary(const GeomBaseNode::Vertices& v, VertexList& path);

    protected:
        GeomHeadNode* render_node_;
        //VertexList path_;

        // 360도 아크를 표현할 세그먼트 갯수
        uint32 num_segments_;
    };
}
}


