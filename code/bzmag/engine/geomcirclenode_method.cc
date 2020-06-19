#define _USE_MATH_DEFINES
#include <cmath>

#include "geomcirclenode.h"
#include "core/methodbinder.h"

using namespace bzmag;
using namespace bzmag::engine;

//----------------------------------------------------------------------------
static void GeomCircleNode_v_setParameters_ss(GeomCircleNode* self, Parameter* param)
{
    self->setParameters(
        param->in()->get<String>(0),
        param->in()->get<String>(1));
}

//----------------------------------------------------------------------------
void GeomCircleNode::bindMethod()
{
    BIND_METHOD(v_setParameters_ss, GeomCircleNode_v_setParameters_ss);
}
