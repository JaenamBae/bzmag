#include "csnode.h"
#include "expression.h"

#include "core/vector2.h"
#include "core/simplepropertybinder.h"
#include "core/nodeeventpublisher.h"

using namespace bzmag;
using namespace bzmag::engine;

IMPLEMENT_CLASS(CSNode, Node);

//----------------------------------------------------------------------------
CSNode::CSNode() : sorigin_("0,0"), sangle_("0")
{
    uint32 key = getID();

    ox_ = new Expression();
    oy_ = new Expression();
    angle_ = new Expression();

    ox_->setKey("ox_" + std::to_string(key));
    oy_->setKey("oy_" + std::to_string(key));
    angle_->setKey("angle_" + std::to_string(key));
}

//----------------------------------------------------------------------------
CSNode::~CSNode()
{

}

//----------------------------------------------------------------------------
bool CSNode::setParameters(const String& origin, const String& angle)
{
    if (ox_.invalid() || oy_.invalid() || angle_.invalid()) 
        return false;

    // 이전값 임시 저장
    const String& pox = ox_->getExpression();
    const String& poy = oy_->getExpression();
    const String& pangle = angle_->getExpression();

    // point 를 ','로 분리해 x,y 값을 얻는다
    Tokenizer token_pt(origin.c_str(), ',');

    // (x,y) 로 분리되지 않으면 실패
    if (token_pt.size() != 2) return false;

    // 분리된 경우 x,y 값(스트링) 저장
    String ox = token_pt[0];
    String oy = token_pt[1];

    if (!ox_->setExpression(ox)
        || !oy_->setExpression(oy)
        || !angle_->setExpression(angle)) {
        ox_->setExpression(pox);
        oy_->setExpression(poy);
        angle_->setExpression(pangle);

        return false;
    }

    sorigin_ = origin;
    sangle_ = angle;

    return update();
}

//----------------------------------------------------------------------------
float64 CSNode::getGlobalOriginX() const
{
    double h13 = CGAL::to_double(transform_.hm(0, 2));  //orgx
    return h13;
}

//----------------------------------------------------------------------------
float64 CSNode::getGlobalOriginY() const
{
    double h23 = CGAL::to_double(transform_.hm(1, 2));  //orgy
    return h23;
}

//----------------------------------------------------------------------------
float64 CSNode::getGlobalAngle() const
{
    double h11 = CGAL::to_double(transform_.hm(0, 0));  //cosd(theta) 
    double h12 = CGAL::to_double(transform_.hm(0, 1));  //sind(theta)
    double h21 = CGAL::to_double(transform_.hm(1, 0));  //-sind(theta) 
    double h22 = CGAL::to_double(transform_.hm(1, 1));  //cosd(theta)
    double angle = atan2(h21, h22);
    return angle;
}

//----------------------------------------------------------------------------
Transformation& CSNode::transformation()
{
    return transform_;
}

//----------------------------------------------------------------------------
void CSNode::insertReferenceNode(Node* node)
{
    CitingIter it = std::find(citingNodes_.begin(), citingNodes_.end(), node);
    if (it == citingNodes_.end()) citingNodes_.push_back(node);
}

//----------------------------------------------------------------------------
void CSNode::removeReferenceNode(Node* node)
{
    CitingIter it = std::find(citingNodes_.begin(), citingNodes_.end(), node);
    if (it != citingNodes_.end()) citingNodes_.erase(it);
}

//----------------------------------------------------------------------------
bool CSNode::update()
{
    if (ox_.invalid() || oy_.invalid() || angle_.invalid()) 
        return false;

    float64 ox = ox_->eval();
    float64 oy = oy_->eval();
    float64 angle = angle_->eval();

    /* https://searchcode.com/file/91859549/3rdparty/CGAL-4.4/demo/Nef_3/include/CGAL/Nef_3/demo_stack.h */
    NT diry = std::sin(angle) * 256 * 256 * 256;
    NT dirx = std::cos(angle) * 256 * 256 * 256;
    NT sin_alpha;
    NT cos_alpha;
    NT w;
    CGAL::rational_rotation_approximation(dirx, diry,
        sin_alpha, cos_alpha, w,
        NT(1), NT(1000000));

    Transformation rotate(CGAL::ROTATION, sin_alpha, cos_alpha, w);
    Transformation translate(CGAL::TRANSLATION, Vector_2(ox, oy));

    // 좌표변환 순서가 rotate --> translate 이어야 함
    transform_ = translate * rotate;

#ifdef _DEBUG	// 디버깅용... 계산값 확인을 위해서...
    Point_2 dp1 = transform_(Point_2(0, 100));
    Vector2 ddp1(CGAL::to_double(dp1.x()), CGAL::to_double(dp1.y()));
#endif

    CSNode* cs_parent = dynamic_cast<CSNode*>(getParent());
    if (cs_parent) {
        Transformation ptrans = cs_parent->transformation();

//         std::ostringstream os;
//         ptrans.print(os);
//         String msg1 = os.str();
// 
//         transform_.print(os);
//         String msg2 = os.str();

        transform_ = ptrans * transform_;
    }

#ifdef _DEBUG	// 디버깅용... 계산값 확인을 위해서...
    Point_2 dp2 = transform_(Point_2(0, 100));
    Vector2 ddp2(CGAL::to_double(dp2.x()), CGAL::to_double(dp2.y()));
#endif

    // CS 자식 노드들을 모두 업데이트 시켜놓고
    for (NodeIterator n = firstChildNode();
        n != lastChildNode(); ++n)
    {
        Node* nn = *n;
        CSNode* cs = dynamic_cast<CSNode*>(nn);
        if (cs) cs->update();
    }

    // CS노드를 참조하는 모든 Geom 노드를 업데이트 한다
    CitingIter it;
    for (it = citingNodes_.begin(); it != citingNodes_.end(); ++it)
    {
        Node* nn = *it;
        nn->update();
    }

    return true;
}

//----------------------------------------------------------------------------
void CSNode::onAttachTo(Node* parent)
{
    update();
}

//----------------------------------------------------------------------------
void CSNode::onDetachFrom(Node* parent)
{
    update();
}

//----------------------------------------------------------------------------
void CSNode::bindProperty()
{
    BIND_PROPERTY(const String&, Origin, &setOrigin, &getOrigin);
    BIND_PROPERTY(const String&, Angle, &setAngle, &getAngle);
}
