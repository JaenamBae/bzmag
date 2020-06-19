#define _USE_MATH_DEFINES
#include <cmath>

#include "geomvertex.h"
#include "core/methodbinder.h"

using namespace bzmag;
using namespace bzmag::engine;

//----------------------------------------------------------------------------
static void GeomVertex_v_setParameters_ss(GeomVertex* self, Parameter* param)
{
    self->setParameters(
        param->in()->get<String>(0),
        param->in()->get<String>(1));
}

//----------------------------------------------------------------------------
void GeomVertex::bindMethod()
{
    BIND_METHOD(v_setParameters_ss, GeomVertex_v_setParameters_ss);
}
