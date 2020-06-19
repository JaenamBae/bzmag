#include "core/methodbinder.h"
#include "geom_to_polytriangle.h"

using namespace bzmag;
using namespace bzmag::engine;

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_v_makePolyStructure_s(GeomToPolyTriangle* self, Parameter* param)
{
    self->makePolyStructures(param->in()->get<String>(0));
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_i_getNumberOfPolyHoles_v(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->get<uint64>(0) = self->getNumberOfPolyHoles();
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_i_getPolyHolesID_i(GeomToPolyTriangle* self, Parameter* param)
{
    uint32 index = param->in()->get<uint32>(0);
    const PolyHolesHelper* polyholes = self->getPolyHoles(index);
    param->out()->get<uint32>(0) = polyholes->ID_;
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_d_getPolyHolesMeshArea_i(GeomToPolyTriangle* self, Parameter* param)
{
    uint32 index = param->in()->get<uint32>(0);
    const PolyHolesHelper* polyholes = self->getPolyHoles(index);
    if (polyholes)
        param->out()->get<float64>(0) = polyholes->area_element_;
    else
        param->out()->get<float64>(0) = 0;
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_d_getDomainArea_v(GeomToPolyTriangle* self, Parameter* param)
{
    float64 meshArea = self->getDomainArea();
    param->out()->get<float64>(0) = meshArea;
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_z_getVertices_I(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->clear();
    uint32 index = param->in()->get<uint32>(0);
    if (index < self->getNumberOfPolyHoles())
    {
        const PolyHolesHelper* polyholes = self->getPolyHoles(index);
        if(polyholes == nullptr) return;

        const Vertices& vertics = polyholes->vertices_;
        Vertices::const_iterator vit;
        for (vit = vertics.begin(); vit != vertics.end(); ++vit)
        {
            const Vector2& ss = vit->second;
            param->out()->add<float64>(ss.x_);
            param->out()->add<float64>(ss.y_);
        }

        /* 2019.09.11 궂이... 대신에 커브를 나눌때 짝수개로 나눠서 중앙점이 포함될 수 있도록 하였음

        // 보조 절점(곡선의 센터) 추가 (중복된 점이 추가될 수도 있음)
        // 참고) 보조절점은 중복제거를 하지 않았음
        // Python 코드에서 어차피 중복제거를 할 것이기에 신경 안쓰기로 함
        // 단, 보조 절점이 메쉬 생성 영역에 포함되어 있어야 함 --> hitTest로 알아냄
        std::list<Vector2>::const_iterator it;
        for (it = self->firstAssistedPoint(); it != self->lastAssistedPoint(); ++it)
        {
            const Vector2& pt = *it;
            if (self->hitTest(pt))
            {
                param->out()->add<float64>(pt.x_);
                param->out()->add<float64>(pt.y_);
            }
        }
        */
    }
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_z_getSegments_I(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->clear();
    uint32 index = param->in()->get<uint32>(0);
    if (index < self->getNumberOfPolyHoles())
    {
        const PolyHolesHelper* polyholes = self->getPolyHoles(index);
        if (polyholes == nullptr) return;

        const Segments& segment = polyholes->segments_;
        Segments::const_iterator sit;
        for (sit = segment.begin(); sit != segment.end(); ++sit)
        {
            const Tuple2<uint32>& ss = sit->second;
            param->out()->add<float64>(ss[0]);
            param->out()->add<float64>(ss[1]);
        }
    }
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_z_getHoles_I(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->clear();
    uint32 index = param->in()->get<uint32>(0);
    if (index < self->getNumberOfPolyHoles())
    {
        const PolyHolesHelper* polyholes = self->getPolyHoles(index);
        if (polyholes == nullptr) return;

        const std::list<Vector2>& vertics = polyholes->holepoints_;
        std::list<Vector2>::const_iterator vit;
        for (vit = vertics.begin(); vit != vertics.end(); ++vit)
        {
            const Vector2& ss = *vit;
            param->out()->add<float64>(ss.x_);
            param->out()->add<float64>(ss.y_);
        }
    }
}

//----------------------------------------------------------------------------
static void GeomToPolyTriangle_z_getPointsOnHoles_v(GeomToPolyTriangle* self, Parameter* param)
{
    param->out()->clear();
    std::list<Vector2> result;
    self->getPointsOnHoles(result);

    std::list<Vector2>::const_iterator it;
    for (it = result.begin(); it != result.end(); ++it)
    {
        const Vector2& ss = *it;
        param->out()->add<float64>(ss.x_);
        param->out()->add<float64>(ss.y_);
    }
}

//----------------------------------------------------------------------------
void GeomToPolyTriangle::bindMethod()
{
    BIND_METHOD(v_makePolyStructure_s, GeomToPolyTriangle_v_makePolyStructure_s);
    BIND_METHOD(i_getNumberOfPolyHoles_v, GeomToPolyTriangle_i_getNumberOfPolyHoles_v);
    BIND_METHOD(i_getPolyHolesID_i, GeomToPolyTriangle_i_getPolyHolesID_i);
    BIND_METHOD(d_getPolyHolesMeshArea_i, GeomToPolyTriangle_d_getPolyHolesMeshArea_i);
    BIND_METHOD(d_getDomainArea_v, GeomToPolyTriangle_d_getDomainArea_v);

    BIND_METHOD(z_getVertices_I, GeomToPolyTriangle_z_getVertices_I);
    BIND_METHOD(z_getSegments_I, GeomToPolyTriangle_z_getSegments_I);
    BIND_METHOD(z_getHoles_I, GeomToPolyTriangle_z_getHoles_I);
    BIND_METHOD(z_getPointsOnHoles_v, GeomToPolyTriangle_z_getPointsOnHoles_v);
}
