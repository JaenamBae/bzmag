#include "geomrectnode.h"
#include "geomheadnode.h"
#include "csnode.h"
#include "expression.h"

#include "core/kernel.h"
#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomRectNode, GeomBaseNode);

//----------------------------------------------------------------------------
GeomRectNode::GeomRectNode() 
    : spoint_("0,0"), swidth_("0"), sheight_("0")
{
    uint32 key = getID();

    px_ = new Expression();
    py_ = new Expression();
    width_  = new Expression();
    height_ = new Expression();

    px_->setKey("px_" + std::to_string(key));
    py_->setKey("py_" + std::to_string(key));
    width_->setKey("width_" + std::to_string(key));
    height_->setKey("height_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomRectNode::~GeomRectNode()
{

}

//----------------------------------------------------------------------------
bool GeomRectNode::setParameters(const String& point,
    const String& dx,
    const String& dy)
{
    if (px_.invalid() || py_.invalid()
        || width_.invalid()
        || height_.invalid())
    {
        return false;
    }

    // ������ �ӽ� ����
    const String& ppx = px_->getExpression();
    const String& ppy = py_->getExpression();
    const String& pdx = width_->getExpression();
    const String& pdy = height_->getExpression();

    // point �� ','�� �и��� x,y ���� ��´�
    Tokenizer token_pt(point.c_str(), ',');

    // (x,y) �� �и����� ������ ����
    if (token_pt.size() != 2) return false;

    // �и��� ��� x,y ��(��Ʈ��) ����
    String px = token_pt[0];
    String py = token_pt[1];

    // �ϴ� Expression���� ��ȯ �õ�
    if (!px_->setExpression(px) || 
        !py_->setExpression(py) ||
        !width_->setExpression(dx) ||
        !height_->setExpression(dy)) 
    {
        // ���н� ������ ����
        px_->setExpression(ppx);
        py_->setExpression(ppy);
        width_->setExpression(pdx);
        height_->setExpression(pdy);

        return false;
    }

    // �ɹ� ���ο� ������ ������Ʈ
    spoint_   = point;
    swidth_   = dx;
    sheight_  = dy;

    return update();
}

//----------------------------------------------------------------------------
bool GeomRectNode::hitTest(float64 x, float64 y)
{
    return false;
}

//----------------------------------------------------------------------------
bool GeomRectNode::update()
{
    if (px_.invalid() || py_.invalid()
        || width_.invalid()
        || height_.invalid())
    {
        return false;
    }

    Transformation transform;
    if (cs_.valid()) {
        transform = cs_->transformation();
    }

    // Construct the polygon.
    float64 px = px_->eval();
    float64 py = py_->eval();
    float64 dx = width_->eval();
    float64 dy = height_->eval();

    // �ݽð�������� ����Ʈ ����
    Point_2 p1(px, py);
    Point_2 p2(px + dx, py);
    Point_2 p3(px+dx, py+dy);
    Point_2 p4(px, py + dy);
    if ((dx < 0 && dy > 0) || (dx > 0 && dy < 0))
    {
        std::swap(p2, p4);
    }

    Rectangle_2 rect(p1, p3);
    if (!rect.is_degenerate()) {
        curves_.clear();

        p1 = transform(p1);
        p2 = transform(p2);
        p3 = transform(p3);
        p4 = transform(p4);

        X_monotone_curve_2 s1(p1, p2);    curves_.push_back(s1);
        X_monotone_curve_2 s2(p2, p3);    curves_.push_back(s2);
        X_monotone_curve_2 s3(p3, p4);    curves_.push_back(s3);
        X_monotone_curve_2 s4(p4, p1);    curves_.push_back(s4);
    }

    return GeomBaseNode::update();
}

//----------------------------------------------------------------------------
void GeomRectNode::bindProperty()
{
    BIND_PROPERTY(const String&, Point, &setPoint, &getPoint);
    BIND_PROPERTY(const String&, Width, &setWidth, &getWidth);
    BIND_PROPERTY(const String&, Height, &setHeight, &getHeight);
}
