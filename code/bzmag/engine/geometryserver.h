#pragma once

/*
Description : Geometry Server
Last Update : 2019.04.23
Author : Jaenam Bae (jaenam@dongyang.ac.kr)
*/

#include "Expression.h"
#include "core/singleton.h"
#include "core/singleton3.h"
#include "geombasenode.h"
#include <map>
#include <list>


namespace bzmag
{
namespace engine
{
    class GeometryServer : public Node, public Singleton3<GeometryServer>
    {
    public:
        GeometryServer(void);
        virtual ~GeometryServer(void);
        DECLARE_CLASS(GeometryServer, Node);


        typedef std::pair<uint32, X_monotone_curve_2> CurveEntity;
        typedef std::pair<uint32, Point_2> PointEntity;

        typedef std::list<CurveEntity> CurveList;
        typedef std::list<PointEntity> PointList;

        typedef std::map<GeomBaseNode*, CurveList> CurvesSets;
        typedef std::map<GeomBaseNode*, PointList> PointsSets;


    public:
        void updateHead(GeomHeadNode* head);


    private:
        CurvesSets curves_sets_;
        PointsSets points_sets_;
    };

}
}