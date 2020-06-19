#include "geombasenode.h"
#include "geomheadnode.h"
#include "csnode.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_ABSTRACTCLASS(GeomBaseNode, Node);



//----------------------------------------------------------------------------
GeomBaseNode::GeomBaseNode() :
    head_(nullptr), 
    geometry_(nullptr), 
    bHead_(false)
{

}

//----------------------------------------------------------------------------
GeomBaseNode::~GeomBaseNode()
{
    if (geometry_) delete geometry_;
    geometry_ = 0;

    curves_.clear();
    vertices_.clear();

    if (cs_.valid()) {
        cs_->removeReferenceNode(this);
    }
}

//----------------------------------------------------------------------------
void GeomBaseNode::setReferedCS(Node* cs)
{
    if (cs_.valid()) {
        cs_->removeReferenceNode(this);
    }
    cs_ = cs;
    if (cs_.valid()) {
        cs_->insertReferenceNode(this);
    }

    update();
}

//----------------------------------------------------------------------------
Node* GeomBaseNode::getReferedCS() const
{
    return cs_;
}

//----------------------------------------------------------------------------
Polygon_set_2* GeomBaseNode::getPolyset()
{
    return geometry_;
}

//----------------------------------------------------------------------------
const GeomBaseNode::Curves& GeomBaseNode::getCurves()
{
    return curves_;
}

//----------------------------------------------------------------------------
const GeomBaseNode::Vertices& GeomBaseNode::getVertices()
{
    return vertices_;
}

//----------------------------------------------------------------------------
GeomHeadNode* GeomBaseNode::getHeadNode()
{
    if(!bHead_) return head_;
    else return nullptr;
}

//----------------------------------------------------------------------------
// 이하 문제 있음 ; 2017.10.08 ; 해결?
bool GeomBaseNode::update()
{
    // 2019.09.12 ; 형상정보의 커브(선)와 절점을 인덱싱 한다
    indexingGeometry();


    size_t num_child = 0;

    // 자식노드들을 업데이트 하는데 GeomBaseNode를 상속받은 class에 한한다
    for (NodeIterator n = firstChildNode(); n != lastChildNode(); ++n)
    {
        Node* node = *n;
        GeomBaseNode* child = dynamic_cast<GeomBaseNode*>(node);
        if (child) {
            // 자신의 해드 노드는 자식의 헤드노드임
            child->head_ = head_;

            // 자식노드를 업데이트 함
            child->update();

            // GeomBaseNode 상속 클래스 중 GeomHeadNode를 제외하고 자식수 카운팅
            //if(child->getGeometryType() != GeomBaseNode::HEAD) num_child++;
            if (!child->isKindOf("GeomHeadNode")) num_child++;
        }
    }

    // 업데이트 할 자식노드가 0이면 즉, 없다면 자신이 최종 노드이다.
    // 이 경우 나의 해드노드의 최종노드로 자신을 설정한다
    if ((0 == num_child) && head_) {
        // 해드노드에 최종노드가 자신임을 통지함
        head_->setLastNode(this);
    }

    return true;
}

//----------------------------------------------------------------------------
void GeomBaseNode::onAttachTo(Node* parent)
{
    GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(parent);
    GeomBaseNode* pp = dynamic_cast<GeomBaseNode*>(parent);

    // 부모노드가 GeomHeadNode이면 head_값 설정
    if (head) {
        head_ = head;
    }

    // 그렇지 않으면 부모노의 해드노드를 현재노드의 해드노느로 설정
    else if (pp) {
        head_ = pp->getHeadNode();
    }

    update();
}

//----------------------------------------------------------------------------
void GeomBaseNode::onDetachFrom(Node* parent)
{
    GeomBaseNode* pp = dynamic_cast<GeomBaseNode*>(parent);
    
    if(head_) {
        // 현재노드의 부모노드와 현재노드의 해드노드가 같지 않다면
        // 현재노드의 해드노드의 최종 노드를 현재노드의 부모노드로 설정
        if(head_ != pp) head_->setLastNode(pp);

        // 현재노드의 부모노드와 현재노드의 해드노드가 같다면
        // 현재노드의 해드노드의 최종노드는 없는 것으로...
        else head_->setLastNode(nullptr);
    }

    // detaching 이므로 현재노드의 해드노드는 없는 것으로 셋팅
    head_ = nullptr;
    update();
}

//----------------------------------------------------------------------------
void GeomBaseNode::indexingGeometry()
{
    // 최종 형상을 이루는 점/선에 대한 인덱싱
    if (geometry_)
    {
        // 기존데이터 삭제
        curves_.clear();
        vertices_.clear();

        // Polygon_set의 각 polygon_with_hole 에 대해서
        // 점과 선을 추출함
        std::list<Polygon_with_holes_2> res;
        std::list<Polygon_with_holes_2>::const_iterator it;
        geometry_->polygons_with_holes(std::back_inserter(res));

        for (it = res.begin(); it != res.end(); ++it)
        {
            Polygon_with_holes_2 polyhole = *it;
            if (!polyhole.is_unbounded()) {
                Polygon_2& poly_o = polyhole.outer_boundary();

                // 여기서 function call을 통해 Polygon을 이루는 점/선에 대한 인덱싱 작업을 한다
                indexingPolygon(poly_o);

                Polygon_with_holes_2::Hole_iterator hit;
                for (hit = polyhole.holes_begin(); hit != polyhole.holes_end(); ++hit) {
                    Polygon_2& poly_i = *hit;
                    indexingPolygon(poly_i);
                }
            }
        }
    }
}

//----------------------------------------------------------------------------
void GeomBaseNode::indexingPolygon(Polygon_2& poly)
{
    Polygon_2::Curve_const_iterator it;
    for (it = poly.curves_begin(); it != poly.curves_end(); ++it) 
    {
        const X_monotone_curve_2& curve = *it;

        Traits_2::Point_2 source = curve.source();
        Traits_2::Point_2 target = curve.target();

        curves_.emplace_back(curve);

        // 이하 코드가 맞는지..? // 2019.09.14
        if(curve.orientation() == CGAL::COUNTERCLOCKWISE)  
            vertices_.emplace_back(source);
        else 
            vertices_.emplace_back(target);
    }
}

//----------------------------------------------------------------------------
void GeomBaseNode::bindProperty()
{
    BIND_PROPERTY(Node*, CoordinateSystem,
        &setReferedCS,
        &getReferedCS);
}
