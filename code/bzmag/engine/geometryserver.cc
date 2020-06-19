#include "GeometryServer.h"
#include "geomheadnode.h"
#include "core/string.h"
#include "core/autoreleasepool.h"

using namespace bzmag;
using namespace bzmag::engine;


IMPLEMENT_CLASS(GeometryServer, Node);

//----------------------------------------------------------------------------
GeometryServer::GeometryServer(void)
{

}

//----------------------------------------------------------------------------
GeometryServer::~GeometryServer(void)
{

}


//----------------------------------------------------------------------------
void GeometryServer::updateHead(GeomHeadNode* head)
{
    // Geometry을 구성하는 선과, 점 관리
    Polygon_set_2* geometry = head->getGeometry();

    uint32 curveID = 0;
    uint32 vertexID = 0;
    if (geometry)
    {
        // 아래줄은 무슨역할을 하는지 잘 모르겠지만 그냥 적어놨다
        geometry->remove_redundant_edges();

        // polygons_with_holes --> 리스트 형태의 polygon_with_holes로 변환
        std::list<Polygon_with_holes_2> res;
        geometry->polygons_with_holes(std::back_inserter(res));

        // 각 polygon_wiht_holes에 대하여 Curve, Vertex 정보 생성
        std::list<Polygon_with_holes_2>::iterator it2;
        for (it2 = res.begin(); it2 != res.end(); ++it2)
        {
            Polygon_with_holes_2& polyhole = *it2;
        }
    }
    return;
}