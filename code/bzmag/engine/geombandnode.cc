#include "geombandnode.h"
#include "geomheadnode.h"
#include "csnode.h"
#include "expression.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomBandNode, GeomSurfaceNode);

//----------------------------------------------------------------------------
GeomBandNode::GeomBandNode() : scx_("0"), scy_("0"), sradii_("0"), swidth_("0")
{
    uint32 key = getID();

    cx_ = new Expression();
    cy_ = new Expression();
    radii_ = new Expression();
    width_ = new Expression();

    cx_->setKey("cx_" + std::to_string(key));
    cy_->setKey("cy_" + std::to_string(key));
    radii_->setKey("radii_" + std::to_string(key));
    width_->setKey("width_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomBandNode::~GeomBandNode()
{

}

//----------------------------------------------------------------------------
bool GeomBandNode::setParameters(const String& x, const String& y, const String& radius, const String& width)
{
    if (cx_.invalid() || cy_.invalid() || radii_.invalid() || width_.invalid()) return false;

    if (!cx_->setExpression(x)
        || !cy_->setExpression(y)
        || !radii_->setExpression(radius)
        || !width_->setExpression(width))
    {
        cx_->setExpression(scx_);
        cy_->setExpression(scy_);
        radii_->setExpression(sradii_);
        width_->setExpression(swidth_);

        return false;
    }

    scx_ = x;
    scy_ = y;
    sradii_ = radius;
    swidth_ = width;

    return update();
}

//----------------------------------------------------------------------------
Vector2 GeomBandNode::getCenter() const
{
    Expression* cx = cx_;
    Expression* cy = cy_;

    float64 x = 0, y = 0;
    if (cx_.valid()) x = cx->eval();
    if (cy_.valid()) y = cy->eval();

    return Vector2(x, y);
}

//----------------------------------------------------------------------------
float64 GeomBandNode::getCenterX() const
{
    Expression* x = cx_;
    return x->eval();
}

//----------------------------------------------------------------------------
float64 GeomBandNode::getCenterY() const
{
    Expression* y = cy_;
    return y->eval();
}

//----------------------------------------------------------------------------
float64 GeomBandNode::getRadius() const
{
    Expression* rad = radii_;
    return rad->eval();
}

//----------------------------------------------------------------------------
float64 GeomBandNode::getWidth() const
{
    Expression* width = width_;
    return width->eval();
}


//----------------------------------------------------------------------------
bool GeomBandNode::hitTest(float64 x, float64 y)
{
    return false;
}

//----------------------------------------------------------------------------
bool GeomBandNode::cover()
{
    Transformation transform;
    if (cs_.valid()) {
        transform = cs_->transformation();
    }

    if (geometry_) delete geometry_;
    geometry_ = 0;

    float64 cx = cx_->eval();
    float64 cy = cy_->eval();
    float64 radii = radii_->eval();
    float64 width = width_->eval();

    float64 inner_radii = abs(radii) - abs(width)*0.5;
    float64 outer_radii = abs(radii) + abs(width)*0.5;

    Point_2 center = transform(Point_2(cx, cy));
    //Point_2 center(cx, cy);
    Circle_2 inner_circle = Circle_2(center, inner_radii*inner_radii);
    Circle_2 outer_circle = Circle_2(center, outer_radii*outer_radii);
    //inner_circle = inner_circle.orthogonal_transform(transform);
    //outer_circle = outer_circle.orthogonal_transform(transform);

    if (inner_circle.is_degenerate() || outer_circle.is_degenerate()) {
        bCoverd_ = false;
        return false;
    }
    else {
        // Construct the polygon.
        Polygon_2 poly_outer = construct_polygon(outer_circle);
        Polygon_2 poly_inner = construct_polygon(inner_circle);

        geometry_ = new Polygon_set_2(poly_outer);
        geometry_->difference(poly_inner);

        bCoverd_ = true;
        return true;
    }
}

//----------------------------------------------------------------------------
void GeomBandNode::bindProperty()
{
    BIND_PROPERTY(const String&, XofCenter, &setCenterX, &getCenterXasString);
    BIND_PROPERTY(const String&, YofCenter, &setCenterY, &getCenterYasString);
    BIND_PROPERTY(const String&, Radius, &setRadius, &getRadiusasString);
    BIND_PROPERTY(const String&, Width, &setWidth, &getWidthasString);
}
