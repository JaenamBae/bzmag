#include "core/methodbinder.h"
#include "geom_to_triangle.h"

using namespace bzmag;
using namespace bzmag::engine;


//----------------------------------------------------------------------------
static void GeomToPolyTriangle_v_convert_s(GeomToPolyTriangle* self, Parameter* param)
{
    self->convert(param->in()->get<String>(0));
}


//----------------------------------------------------------------------------
static void GeomToPolyTriangle_z_getVertices_v(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->clear();

    GeomToPolyTriangle::Vertices::const_iterator vit;
    for (vit = self->firstVertex(); vit != self->lastVertex(); ++vit)
    {
        const Vector2& ss = vit->second;

        //param->out()->add<uint32>(vit->first);
        param->out()->add<float64>(ss.x_);
        param->out()->add<float64>(ss.y_);
    }
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_z_getSegments_v(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->clear();

    GeomToPolyTriangle::Segments::const_iterator sit;
    for (sit = self->firstSegment(); sit != self->lastSegment(); ++sit)
    {
        const Tuple2<uint32>& ss = sit->second;

        //param->out()->add<uint32>(sit->first);
        param->out()->add<uint32>(ss.x_);
        param->out()->add<uint32>(ss.y_);
    }
}

//----------------------------------------------------------------------------
void GeomToPolyTriangle::bindMethod()
{
    BIND_METHOD(v_convert_s, GeomToPolyTriangle_v_convert_s);
    BIND_METHOD(z_getVertices_v, GeomToPolyTriangle_z_getVertices_v);
    BIND_METHOD(z_getSegments_v, GeomToPolyTriangle_z_getSegments_v);
}
