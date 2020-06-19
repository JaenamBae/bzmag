#include "geomvertex.h"
#include "expression.h"
#include "csnode.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(GeomVertex, Object);

//----------------------------------------------------------------------------
GeomVertex::GeomVertex() : sx_("0"), sy_("0")
{
    uint32 key = getID();

    x_ = new Expression();
    y_ = new Expression();

    x_->setKey("x_" + std::to_string(key));
    y_->setKey("y_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomVertex::~GeomVertex()
{

}

//----------------------------------------------------------------------------
bool GeomVertex::setParameters(const String& x, const String& y)
{
    if (x_.invalid() || y_.invalid()) return false;

    if (!x_->setExpression(x)
        || !y_->setExpression(y))
    {
        x_->setExpression(sx_);
        y_->setExpression(sy_);

        return false;
    }

    sx_ = x;
    sy_ = y;

    return true;
}

//----------------------------------------------------------------------------
float64 GeomVertex::getX() const
{
    Expression* x = x_;
    return x->eval();
}

//----------------------------------------------------------------------------
float64 GeomVertex::getY() const
{
    Expression* y = y_;
    return y->eval();
}

//----------------------------------------------------------------------------
void GeomVertex::bindProperty()
{
    BIND_PROPERTY(const String&, x, &setX, &getXasString);
    BIND_PROPERTY(const String&, y, &setY, &getYasString);
}
