#include "geomheadnode.h"
#include "geom_to_path.h"
#include "core/methodbinder.h"

using namespace bzmag;
using namespace bzmag::engine;

//----------------------------------------------------------------------------
// static void GeomHeadNode_v_setNormalDeviation_k(GeomHeadNode* self, Parameter* param)
// {
//     self->setNormalDeviation(param->in()->get<uint32>(0));
// }

//----------------------------------------------------------------------------
static void GeomHeadNode_z_getPath_i(GeomHeadNode* self, Parameter* param)
{
    param->out()->clear();
    uint32 normal_deviation = param->in()->get<uint32>(0);

    // CCW direction
    GeomToPath geom_to_path(self);
    if (normal_deviation > 0) geom_to_path.setNormalDeviation(normal_deviation);

    GeomToPath::VertexList vertices;
    geom_to_path.makePath(vertices);

    GeomToPath::VertexList::const_iterator it;
    for (it = vertices.begin(); it != vertices.end(); ++it)
    {
        GeomToPath::VertexInfo pt = (*it);
        param->out()->add<float64>(pt.x);
        param->out()->add<float64>(pt.y);
        param->out()->add<uint32>(pt.cmd);
    }
}

//----------------------------------------------------------------------------
void GeomHeadNode::bindMethod()
{
    BIND_METHOD(z_getPath_i, GeomHeadNode_z_getPath_i);
}
