#include "geomcurvenode.h"
#include "expression.h"
#include "csnode.h"

#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

#include <CGAL/Circular_kernel_intersections.h>
#include <CGAL/Circular_arc_point_2.h>

using namespace bzmag;
using namespace bzmag::engine;

typedef CGAL::Point_2<K>                 Pt2;
typedef CGAL::Circle_2<K>                Circ2;
typedef CGAL::Line_arc_2<K>              LineArc2;
typedef CGAL::Circular_arc_2<K>          CircleArc2;
typedef CGAL::CK2_Intersection_traits<K, Circ2, LineArc2>::type InterResLine;
typedef CGAL::CK2_Intersection_traits<K, Circ2, CircleArc2>::type InterResCircle;

IMPLEMENT_CLASS(GeomCurveNode, GeomBaseNode);


float64 GeomCurveNode::torr_ = 1e-12;

//----------------------------------------------------------------------------
GeomCurveNode::GeomCurveNode() : sstart_("0,0"), send_("0,0"), smid_("")
{
    uint32 key = getID();

    sx_ = new Expression();
    sy_ = new Expression();
    ex_ = new Expression();
    ey_ = new Expression();
    mx_ = new Expression();
    my_ = new Expression();

    sx_->setKey("sx_" + std::to_string(key));
    sy_->setKey("sy_" + std::to_string(key));
    ex_->setKey("ex_" + std::to_string(key));
    ey_->setKey("ey_" + std::to_string(key));
    mx_->setKey("mx_" + std::to_string(key));
    my_->setKey("my_" + std::to_string(key));
}

//----------------------------------------------------------------------------
GeomCurveNode::~GeomCurveNode()
{

}

//----------------------------------------------------------------------------
bool GeomCurveNode::setParameters(const String& start, const String& end, const String& mid/* = ""*/)
{
    if (sx_.invalid() || sy_.invalid()) return false;
    if (ex_.invalid() || ey_.invalid()) return false;
    if (mx_.invalid() || my_.invalid()) return false;

    // point �� ','�� �и��� x,y ���� ��´�
    Tokenizer tok_start(start.c_str(), ',');
    Tokenizer tok_end(end.c_str(), ',');
    Tokenizer tok_mid(mid.c_str(), ',');

    // (x,y) �� �и����� ������ ���� --> 2���� ���ҷ� �и��Ǿ�� ��
    if (tok_start.size() != 2) return false;
    if (tok_end.size() != 2) return false;

    // �߽����� "" ���� ���� �� �����Ƿ� �Ľ��� ���� ������ 1�̰ų� 2�̾�� �Ѵ�
    if ((tok_mid.size() != 2) && (tok_mid.size() != 1)) return false;


    // ������
    // sx, sy�� ������ �ӽ� ����
    const String& psx = sx_->getExpression();
    const String& psy = sy_->getExpression();

    // expression�� ���� �õ�
    if (!sx_->setExpression(tok_start[0])
        || !sy_->setExpression(tok_start[1]))
    {
        // ���н� ������ ����
        sx_->setExpression(psx);
        sy_->setExpression(psy);

        return false;
    }

    // ����
    // ex, ey�� ������ �ӽ� ����
    const String& pex = ex_->getExpression();
    const String& pey = ey_->getExpression();

    // expression�� ���� �õ�
    if (!ex_->setExpression(tok_end[0])
        || !ey_->setExpression(tok_end[1]))
    {
        // ���н� ������ ����
        ex_->setExpression(pex);
        ey_->setExpression(pey);

        return false;
    }

    // �߽����� �ִٸ�...
    if (tok_mid.size() == 2)
    {
        // mx, my�� ������ �ӽ� ����
        const String& pmx = mx_->getExpression();
        const String& pmy = my_->getExpression();

        if (!mx_->setExpression(tok_mid[0])
            || !my_->setExpression(tok_mid[1]))
        {
            // ���н� ������ ����
            mx_->setExpression(pmx);
            my_->setExpression(pmy);

            return false;
        }
    }

    sstart_ = start;
    send_ = end;
    smid_ = mid;

    return update();
}

//----------------------------------------------------------------------------
bool GeomCurveNode::pointOnCurve(float64 x, float64 y) const
{
    Curves::const_iterator it;
    for (it = curves_.begin(); it != curves_.end(); ++it)
    {
        const X_monotone_curve_2& curve = *it;

        Traits_2::Point_2 source = curve.source();
        Traits_2::Point_2 target = curve.target();

        // �������� ������ ���� ����Ʈ�� �ش��ϴ� ��
        Circ2 c(Pt2(x, y), torr_*torr_);

        // curve�� ������ ����
        Pt2 ss(CGAL::to_double(source.x()), CGAL::to_double(source.y()));
        Pt2 tt(CGAL::to_double(target.x()), CGAL::to_double(target.y()));

        if (curve.is_linear())
        {
            std::vector<InterResLine> output;
            LineArc2 l(ss, tt);
            CGAL::intersection(c, l, std::back_inserter(output));

            return (output.size() > 0) ? true : false;
        }
        else
        {
            // Ŀ���� Supporting Circle ����
            Circle_2 circle = curve.supporting_circle();
            NT sqr_radii = circle.squared_radius();
            Point_2 center = circle.center();
            Pt2 cc(center.x(), center.y());
            Circ2 supporting_circle(cc, sqr_radii);
            CircleArc2 a(supporting_circle, ss, tt);


            // �Ʒ� intersection �Լ��� ���� �ڵ���(2016.05.25)���� �ι�° ���� �������� ������ �Ǿ���
            // ����(2016.07.25)�� ù��° ���� �������� ������ ��
            // ���� �̻��ϴ� ����!!
            std::vector<InterResCircle> output;
            CGAL::intersection(a, c, std::back_inserter(output));

            return (output.size() > 0) ? true : false;
        }
    }
    return false;
}

//----------------------------------------------------------------------------
bool GeomCurveNode::hitTest(float64 x, float64 y)
{
    return false;
}

//----------------------------------------------------------------------------
bool GeomCurveNode::update()
{
    if (sx_.invalid() || sy_.invalid() || 
        ex_.invalid() || ey_.invalid() || 
        mx_.invalid() || my_.invalid())
        return false;

    Transformation transform;
    if (cs_.valid()) {
        transform = cs_->transformation();
    }

    float64 sx = sx_->eval();
    float64 sy = sy_->eval();

    float64 ex = ex_->eval();
    float64 ey = ey_->eval();

    float64 mx = mx_->eval();
    float64 my = my_->eval();

    Point_2 ss = transform(Point_2(sx, sy));
    Point_2 ee = transform(Point_2(ex, ey));
    Point_2 mm = transform(Point_2(mx, my));

    Curve_2 curve;
    if (smid_ == "") {
        curve = Curve_2(ss, ee);
    }
    else {
        curve = Curve_2(ss, mm, ee);
    }
    if (curve.source() != curve.target())
    {
        Traits_2 traits;
        std::list<CGAL::Object> objects;
        traits.make_x_monotone_2_object() (curve, std::back_inserter(objects));

        // Construct the curves.
        curves_.clear();
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
void GeomCurveNode::bindProperty()
{
    BIND_PROPERTY(const String&, StartPoint, &setStartPoint, &getStartPoint);
    BIND_PROPERTY(const String&, EndPoint, &setEndPoint, &getEndPoint);
    BIND_PROPERTY(const String&, MidPoint, &setMidPoint, &getMidPoint);
}
