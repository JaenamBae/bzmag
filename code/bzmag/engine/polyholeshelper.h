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

    // 단독으로 존재하는 폴리곤(홀을 포함하는)
    // 즉, 폴리곤의 셋트가 아니란 말씀
    // 이들의 단위로 요소생성을 하고
    // 생성된 요소들을 합쳐 최종 요소를 만들어 낼 것임
    // final_polyholes_ 이 최종 만들어진 폴리곤홀이 될 것이며
    // 이의 면적을 area_polyhole_ 에 저장할 것임
    // 이는 num_elements_와 함께 추후 요소사이즈 컨트롤을 위해 사용될 것임
    // Comments : 2019.04.03
    struct PolyHolesHelper
    {
        uint32 ID_;

        // 원본
        Polygon_with_holes_2 polyholes_;

        // 기저절점을 고려하여 (이웃한 폴리곤을 이루는 절점이 나의 폴리곤에 접해 있는경우)
        // 폴리곤을 구성하는 X_monoton_Curve 을 쪼개는 작업 후 결과
        Polygon_with_holes_2 regen_polyholes_;

        // 절점 일치판단을 위해 절점의 정밀도를 떨어뜨림(오차 1e^-12)
        // 동시에 절점을 세그멘테이션 함
        Polygon_with_holes_2 approx_polyholes_;

        // approx_polyholes_ 내부에 존재하는 폴리곤 셋트들
        Polygon_set_2 tools_;

        //-----------------------------------------------------------------
        // approx_polyholes_ - tools_ --> 최종 요소생성을 위한 단위 폴리곤
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