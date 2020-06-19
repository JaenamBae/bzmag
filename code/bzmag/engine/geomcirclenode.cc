#include "geomcirclenode.h"
#include "geomheadnode.h"
#include "csnode.h"
#include "expression.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomCircleNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomCircleNode::GeomCircleNode() : scenter_("0,0"), sradii_("0")
{
    uint32 key = getID();

    cx_ = new Expression();
    cy_ = new Expression();
    radii_ = new Expression();

    cx_->setKey("cx_" + std::to_string(key));
    cy_->setKey("cy_" + std::to_string(key));
    radii_->setKey("radii_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomCircleNode::~GeomCircleNode()
{

}

//----------------------------------------------------------------------------
bool GeomCircleNode::setParameters(const String& center, const String& radius)
{
    if (cx_.invalid() || cy_.invalid() || radii_.invalid()) return false;

    // ������ �ӽ� ����
    const String& pcx = cx_->getExpression();
    const String& pcy = cy_->getExpression();
    const String& pradii = radii_->getExpression();

    // center �� ','�� �и��� x,y ���� ��´�
    Tokenizer token_pt(center.c_str(), ',');

    // (x,y) �� �и����� ������ ����
    if (token_pt.size() != 2) return false;

    // �и��� ��� x,y ��(��Ʈ��) ����
    String cx = token_pt[0];
    String cy = token_pt[1];

    // �ϴ� Expression���� ��ȯ �õ�
    if (!cx_->setExpression(cx)
        || !cy_->setExpression(cy)
        || !radii_->setExpression(radius))
    {
        cx_->setExpression(pcx);
        cy_->setExpression(pcy);
        radii_->setExpression(pradii);

        return false;
    }

    // �ɹ� ���ο� ������ ������Ʈ
    scenter_ = center;
    sradii_ = radius;

    return update();
}

//----------------------------------------------------------------------------
bool GeomCircleNode::hitTest(float64 x, float64 y)
{
    return false;
}

//----------------------------------------------------------------------------
bool GeomCircleNode::update()
{
    if (cx_.invalid() || cy_.invalid() || radii_.invalid())
        return false;

    Transformation transform;
    if (cs_.valid()) {
        transform = cs_->transformation();
    }

    float64 cx = cx_->eval();
    float64 cy = cy_->eval();
    float64 radii = radii_->eval();

    Point_2 center = transform(Point_2(cx, cy));
    Circle_2 circle(center, radii*radii);

    if (!circle.is_degenerate()) {
        curves_.clear();

        // Subdivide the circle into two x-monotone arcs.
        Traits_2 traits;
        Curve_2 curve(circle);
        std::list<CGAL::Object>  objects;
        traits.make_x_monotone_2_object() (curve, std::back_inserter(objects));
        CGAL_assertion(objects.size() == 2);

        // Insert arcs to the curves_
        X_monotone_curve_2 arc;
        std::list<CGAL::Object>::iterator iter;

        for (iter = objects.begin(); iter != objects.end(); ++iter) {
            CGAL::assign(arc, *iter);
            curves_.push_back(arc);
        }
    }

    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
void GeomCircleNode::bindProperty()
{
    BIND_PROPERTY(const String&, Center, &setCenter, &getCenter);
    BIND_PROPERTY(const String&, Radius, &setRadius, &getRadius);
}
