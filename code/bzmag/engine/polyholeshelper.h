#pragma once

/*
Description : Helper class to Convert Geometries to Mesh Generator (Triangle) Input File Format (*.POLY)
Last Update : 2018.01.15
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "geometric_entity.h"
#include "core/vector2.h"
#include "core/primitivetype.h"

namespace bzmag
{
namespace engine
{
    // [id] <x> <y>
    typedef std::map<uint32, Vector2> Vertices;

    // [id] <endpoint> <endpoint>
    typedef std::map<uint32, Tuple2<uint32>> Segments;

    // �ܵ����� �����ϴ� ������(Ȧ�� �����ϴ�)
    // ��, �������� ��Ʈ�� �ƴ϶� ����
    // �̵��� ������ ��һ����� �ϰ�
    // ������ ��ҵ��� ���� ���� ��Ҹ� ����� �� ����
    // final_polyholes_ �� ���� ������� ������Ȧ�� �� ���̸�
    // ���� ������ area_polyhole_ �� ������ ����
    // �̴� num_elements_�� �Բ� ���� ��һ����� ��Ʈ���� ���� ���� ����
    // Comments : 2019.04.03
    struct PolyHolesHelper
    {
        uint32 ID_;

        // ����
        Polygon_with_holes_2 polyholes_;

        // ���������� ����Ͽ� (�̿��� �������� �̷�� ������ ���� �����￡ ���� �ִ°��)
        // �������� �����ϴ� X_monoton_Curve �� �ɰ��� �۾� �� ���
        Polygon_with_holes_2 regen_polyholes_;

        // ���� ��ġ�Ǵ��� ���� ������ ���е��� ����߸�(���� 1e^-12)
        // ���ÿ� ������ ���׸����̼� ��
        Polygon_with_holes_2 approx_polyholes_;

        // approx_polyholes_ ���ο� �����ϴ� ������ ��Ʈ��
        Polygon_set_2 tools_;

        //-----------------------------------------------------------------
        // approx_polyholes_ - tools_ --> ���� ��һ����� ���� ���� ������
        Polygon_with_holes_2 final_polyholes_;

        Vertices vertices_;
        Segments segments_;
        std::list<Vector2> holepoints_;

        uint32 num_elements_;
        float64 area_polyhole_;
        float64 area_element_;
        //-----------------------------------------------------------------
    };
}
}