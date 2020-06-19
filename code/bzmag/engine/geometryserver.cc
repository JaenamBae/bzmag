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
    // Geometry�� �����ϴ� ����, �� ����
    Polygon_set_2* geometry = head->getGeometry();

    uint32 curveID = 0;
    uint32 vertexID = 0;
    if (geometry)
    {
        // �Ʒ����� ���������� �ϴ��� �� �𸣰����� �׳� �������
        geometry->remove_redundant_edges();

        // polygons_with_holes --> ����Ʈ ������ polygon_with_holes�� ��ȯ
        std::list<Polygon_with_holes_2> res;
        geometry->polygons_with_holes(std::back_inserter(res));

        // �� polygon_wiht_holes�� ���Ͽ� Curve, Vertex ���� ����
        std::list<Polygon_with_holes_2>::iterator it2;
        for (it2 = res.begin(); it2 != res.end(); ++it2)
        {
            Polygon_with_holes_2& polyhole = *it2;
        }
    }
    return;
}