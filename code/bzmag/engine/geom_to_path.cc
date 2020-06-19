#include "geom_to_path.h"

using namespace bzmag;
using namespace bzmag::engine;


//----------------------------------------------------------------------------
GeomToPath::GeomToPath(GeomHeadNode* render_node) :
    num_segments_(90),
    render_node_(render_node)
{

}

//----------------------------------------------------------------------------
GeomToPath::~GeomToPath()
{

}

//----------------------------------------------------------------------------
void GeomToPath::setNormalDeviation(uint32 num)
{
    num_segments_ = num;
}

//----------------------------------------------------------------------------
uint32 GeomToPath::getNormalDeviation() const
{
    return num_segments_;
}

//----------------------------------------------------------------------------
bool GeomToPath::makePath(VertexList& path)
{
    // initialize
    path.clear();

    // Surface type
    Polygon_set_2* geometry = render_node_->getPolyset();
    if (geometry)
    {
        std::list<Polygon_with_holes_2> res;
        geometry->polygons_with_holes(std::back_inserter(res));

        std::list<Polygon_with_holes_2>::iterator it;

        for (it = res.begin(); it != res.end(); ++it)
        {
            Polygon_with_holes_2 pwh = *it;
            Polygon_2 poly_outer = pwh.outer_boundary();
            makeBoundary(poly_outer, path);

            Polygon_with_holes_2::Hole_iterator hi;
            for (hi = pwh.holes_begin(); hi != pwh.holes_end(); ++hi)
            {
                Polygon_2 poly_hole = *(hi);
                makeBoundary(poly_hole, path);
            }
        }
    }

    
    // Curve type
    else if(render_node_->getCurves().size() > 0)
    {
        const GeomBaseNode::Curves& curve = render_node_->getCurves();
        makeBoundary(curve, path);

    }

    // Vertex type
    else if (render_node_->getVertices().size() > 0)
    {
        const GeomBaseNode::Vertices& vertex = render_node_->getVertices();
        makeBoundary(vertex, path);
    }
    


    return true;
}


//----------------------------------------------------------------------------
void GeomToPath::makeBoundary(Polygon_2& p, VertexList& path)
{
    VertexInfo p1, p2, p3;
    Polygon_2::Curve_const_iterator i;
    for (i = p.curves_begin(); i != p.curves_end(); ++i) {
        X_monotone_curve_2 edge = *i;
        Traits_2::Point_2 source = edge.source();
        Traits_2::Point_2 target = edge.target();

        p1.x = CGAL::to_double(source.x());
        p1.y = CGAL::to_double(source.y());
        p1.cmd = 1;     // Move To ; 시작점만 Move To 명령어임

        p2.x = CGAL::to_double(target.x());
        p2.y = CGAL::to_double(target.y());
        p2.cmd = 2;     // Line To

        p3.cmd = 2;     // Line To

        if (i==p.curves_begin()) path.emplace_back(p1);//  ps.move_to(p1.x, p1.y);
        if (!edge.is_linear())
        {
            Circle_2 circle = edge.supporting_circle();
            Point_2 center = circle.center();
            VertexInfo cc;
            cc.x = CGAL::to_double(center.x());
            cc.y = CGAL::to_double(center.y());
            float64 radius = sqrt(CGAL::to_double(circle.squared_radius()));

            float64 start_angle = atan2(p1.y - cc.y, p1.x - cc.x);
            float64 end_angle   = atan2(p2.y - cc.y, p2.x - cc.x);

            if (start_angle < 0) start_angle += 2*CGAL_PI;
            if (end_angle < 0)   end_angle   += 2*CGAL_PI;

            int segnum = 0;
            float64 delang = 0;
            if (edge.orientation() == CGAL::COUNTERCLOCKWISE) {
                if (start_angle > end_angle) start_angle -= 2*CGAL_PI;
            } else {
                if (start_angle < end_angle) end_angle   -= 2*CGAL_PI;
            }

            segnum = int(abs(num_segments_*((start_angle - end_angle) / (2.0*CGAL_PI))));
            delang = (end_angle - start_angle) / segnum;

            for (int sn = 1; sn < segnum; ++sn)
            {
                float64 ang = start_angle + sn*delang;
                p3.x = radius*cos(ang) + cc.x;
                p3.y = radius*sin(ang) + cc.y;
                path.emplace_back(p3);
                //ps.line_to(p3.x, p3.y);
            }
        }
        path.emplace_back(p2);
        //ps.line_to(p2.x, p2.y);
    }
}

//----------------------------------------------------------------------------
void GeomToPath::makeBoundary(const GeomBaseNode::Curves& c, VertexList& path)
{
    VertexInfo p1, p2, p3;
    GeomBaseNode::Curves::const_iterator i;
    for (i = c.begin(); i != c.end(); ++i) {
        X_monotone_curve_2 edge = *i;
        Traits_2::Point_2 source = edge.source();
        Traits_2::Point_2 target = edge.target();

        p1.x = CGAL::to_double(source.x());
        p1.y = CGAL::to_double(source.y());
        p1.cmd = 1;     // Move To ; 시작점만 Move To 명령어임

        p2.x = CGAL::to_double(target.x());
        p2.y = CGAL::to_double(target.y());
        p2.cmd = 2;     // Line To

        p3.cmd = 2;     // Line To

        if (i == c.begin()) path.emplace_back(p1);//  ps.move_to(p1.x, p1.y);
        if (!edge.is_linear())
        {
            Circle_2 circle = edge.supporting_circle();
            Point_2 center = circle.center();
            VertexInfo cc;
            cc.x = CGAL::to_double(center.x());
            cc.y = CGAL::to_double(center.y());
            float64 radius = sqrt(CGAL::to_double(circle.squared_radius()));

            float64 start_angle = atan2(p1.y - cc.y, p1.x - cc.x);
            float64 end_angle = atan2(p2.y - cc.y, p2.x - cc.x);

            if (start_angle < 0) start_angle += 2 * CGAL_PI;
            if (end_angle < 0)   end_angle += 2 * CGAL_PI;

            int segnum = 0;
            float64 delang = 0;
            if (edge.orientation() == CGAL::COUNTERCLOCKWISE) {
                if (start_angle > end_angle) start_angle -= 2 * CGAL_PI;
            }
            else {
                if (start_angle < end_angle) end_angle -= 2 * CGAL_PI;
            }

            segnum = int(abs(num_segments_*((start_angle - end_angle) / (2.0*CGAL_PI))));
            delang = (end_angle - start_angle) / segnum;

            for (int sn = 1; sn < segnum; ++sn)
            {
                float64 ang = start_angle + sn * delang;
                p3.x = radius * cos(ang) + cc.x;
                p3.y = radius * sin(ang) + cc.y;
                path.emplace_back(p3);
                //ps.line_to(p3.x, p3.y);
            }
        }
        path.emplace_back(p2);
        //ps.line_to(p2.x, p2.y);
    }
}

//----------------------------------------------------------------------------
void GeomToPath::makeBoundary(const GeomBaseNode::Vertices& v, VertexList& path)
{
    VertexInfo p1;
    p1.cmd = 1;
    //p1.x = CGAL::to_double(v->x());
    //p1.y = CGAL::to_double(v->y());

    path.emplace_back(p1);
}
