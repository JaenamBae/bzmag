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
// ���� ���� ���� ; 2017.10.08 ; �ذ�?
bool GeomBaseNode::update()
{
    // 2019.09.12 ; ���������� Ŀ��(��)�� ������ �ε��� �Ѵ�
    indexingGeometry();


    size_t num_child = 0;

    // �ڽĳ����� ������Ʈ �ϴµ� GeomBaseNode�� ��ӹ��� class�� ���Ѵ�
    for (NodeIterator n = firstChildNode(); n != lastChildNode(); ++n)
    {
        Node* node = *n;
        GeomBaseNode* child = dynamic_cast<GeomBaseNode*>(node);
        if (child) {
            // �ڽ��� �ص� ���� �ڽ��� �������
            child->head_ = head_;

            // �ڽĳ�带 ������Ʈ ��
            child->update();

            // GeomBaseNode ��� Ŭ���� �� GeomHeadNode�� �����ϰ� �ڽļ� ī����
            //if(child->getGeometryType() != GeomBaseNode::HEAD) num_child++;
            if (!child->isKindOf("GeomHeadNode")) num_child++;
        }
    }

    // ������Ʈ �� �ڽĳ�尡 0�̸� ��, ���ٸ� �ڽ��� ���� ����̴�.
    // �� ��� ���� �ص����� �������� �ڽ��� �����Ѵ�
    if ((0 == num_child) && head_) {
        // �ص��忡 ������尡 �ڽ����� ������
        head_->setLastNode(this);
    }

    return true;
}

//----------------------------------------------------------------------------
void GeomBaseNode::onAttachTo(Node* parent)
{
    GeomHeadNode* head = dynamic_cast<GeomHeadNode*>(parent);
    GeomBaseNode* pp = dynamic_cast<GeomBaseNode*>(parent);

    // �θ��尡 GeomHeadNode�̸� head_�� ����
    if (head) {
        head_ = head;
    }

    // �׷��� ������ �θ���� �ص��带 �������� �ص����� ����
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
        // �������� �θ���� �������� �ص��尡 ���� �ʴٸ�
        // �������� �ص����� ���� ��带 �������� �θ���� ����
        if(head_ != pp) head_->setLastNode(pp);

        // �������� �θ���� �������� �ص��尡 ���ٸ�
        // �������� �ص����� �������� ���� ������...
        else head_->setLastNode(nullptr);
    }

    // detaching �̹Ƿ� �������� �ص���� ���� ������ ����
    head_ = nullptr;
    update();
}

//----------------------------------------------------------------------------
void GeomBaseNode::indexingGeometry()
{
    // ���� ������ �̷�� ��/���� ���� �ε���
    if (geometry_)
    {
        // ���������� ����
        curves_.clear();
        vertices_.clear();

        // Polygon_set�� �� polygon_with_hole �� ���ؼ�
        // ���� ���� ������
        std::list<Polygon_with_holes_2> res;
        std::list<Polygon_with_holes_2>::const_iterator it;
        geometry_->polygons_with_holes(std::back_inserter(res));

        for (it = res.begin(); it != res.end(); ++it)
        {
            Polygon_with_holes_2 polyhole = *it;
            if (!polyhole.is_unbounded()) {
                Polygon_2& poly_o = polyhole.outer_boundary();

                // ���⼭ function call�� ���� Polygon�� �̷�� ��/���� ���� �ε��� �۾��� �Ѵ�
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

        // ���� �ڵ尡 �´���..? // 2019.09.14
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
