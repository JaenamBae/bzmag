#include "graphics_test.h"
#include "resource.h"

#include "core/cwnguard.h"
#include "core/ref.h"
#include "core/kernel.h"
#include "core/exception.h"
#include "core/compositeproperty.h"
#include "engine/expression.h"
#include "engine/csnode.h"
#include "engine/geombasenode.h"
#include "engine/geomcurvenode.h"
#include "engine/geomcirclenode.h"
#include "engine/geomrectnode.h"
#include "engine/geomcoverlinenode.h"
#include "engine/geommovenode.h"
#include "engine/geomrotatenode.h"
#include "engine/geomsubtractnode.h"
#include "engine/geomclonetonode.h"
#include "engine/geomclonefromnode.h"

#include "engine/geom_to_polytriangle.h"
#include "wingraphics/modelerview.h"

using namespace bzmag;
using namespace bzmag::engine;


//-----------------------------------------------------------------------------
BOOL CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    return FALSE;
    //  return DefWindowProc(hwnd, msg, wparam, lparam);
}

void GraphicsTestCase::test_CGAL()
{
    Kernel* kernel = Kernel::instance();

    // Circle1
    GeomHeadNode* head1       = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/usr2/geom1"));
    GeomCircleNode* circle1   = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/usr2/geom1/CreateCircle"));
    GeomCoverLineNode* cover1 = dynamic_cast<GeomCoverLineNode*>(kernel->create("GeomCoverLineNode", "/usr2/geom1/CreateCircle/CoverLine"));

    // Circle2
    GeomHeadNode* head2       = dynamic_cast<GeomHeadNode*>(kernel->create("GeomHeadNode", "/usr2/geom2"));
    GeomCircleNode* circle2   = dynamic_cast<GeomCircleNode*>(kernel->create("GeomCircleNode", "/usr2/geom2/CreateCircle"));
    GeomCoverLineNode* cover2 = dynamic_cast<GeomCoverLineNode*>(kernel->create("GeomCoverLineNode", "/usr2/geom2/CreateCircle/CoverLine"));
    GeomRotateNode* rot       = dynamic_cast<GeomRotateNode*>(kernel->create("GeomRotateNode", "/usr2/geom2/CreateCircle/CoverLine/Rotate"));

    // working
    circle1->setParameters("0, 1", "50.05");
    
    // working
    circle2->setParameters("1, 0", "30.12");

    // working
    rot->setParameters("_pi/2");

    // change attributes (head node) of the two circles
    head1->attach(circle2);
    head2->attach(circle1);
    head2->setColor(Color(0, 255, 0, 200));



    //GeomToPolyTriangle* converter = GeomToPolyTriangle::instance();
    //converter->convert("/usr2");

}


//-----------------------------------------------------------------------------
void GraphicsTestCase::test_Renderer()
{
    HWND hwnd = CreateDialog(
        GetModuleHandle(0),
        MAKEINTRESOURCE(IDD_RENDERTESTDIALOG),
        0,
        DialogProc);
    ShowWindow(hwnd, SW_NORMAL);


    typedef Ref<ModelerView> RefRenderer;
    RefRenderer renderer = Kernel::instance()->create(
        "ModelerView", "/sys/server/renderer");

    // 렌더링뷰의 윈도우 핸들 설정
    renderer->set_window_handle(int(hwnd));

    // 렌더링뷰의 렌더링 경로 설정
    renderer->set_geometry_path("/usr");

    // 렌더링뷰의 실제 사이즈 설정 (윈도우 사이즈 변경될 때 마다 호출해 주어야 함)
    RECT rc;
    GetClientRect(hwnd, &rc);
    renderer->set_window_size(rc.right, rc.bottom);

    typedef Ref<GeomHeadNode> RefHeadNode;
    RefHeadNode head = Kernel::instance()->create(
        "GeomHeadNode", "/usr/geometry");

    typedef Ref<GeomCircleNode> RefCircleNode;
    RefCircleNode circle = Kernel::instance()->create(
        "GeomCircleNode", "/usr/geometry/CreateCircle");

    typedef Ref<GeomCoverLineNode> RefCoverLineNode;
    RefCoverLineNode cover = Kernel::instance()->create(
        "GeomCoverLineNode", "/usr/geometry/CreateCircle/CoverLine");

    circle->setParameters("0, 0", "100");

    renderer->set_model_rect(-100, -100, 100, 100);
    renderer->fit_to_window();

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        renderer->draw();
        Sleep(0);

        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyWindow(hwnd);
}



//-----------------------------------------------------------------------------
void GraphicsTestCase::test_Boolean()
{
    HWND hwnd = CreateDialog(
        GetModuleHandle(0),
        MAKEINTRESOURCE(IDD_RENDERTESTDIALOG),
        0,
        DialogProc);
    ShowWindow(hwnd, SW_NORMAL);


    typedef Ref<ModelerView> RefRenderer;
    RefRenderer renderer = Kernel::instance()->create(
        "ModelerView", "/sys/server/renderer");

    // 렌더링뷰의 윈도우 핸들 설정
    renderer->set_window_handle(int(hwnd));

    // 렌더링뷰의 렌더링 경로 설정
    renderer->set_geometry_path("/usr3");

    // 렌더링뷰의 실제 사이즈 설정 (윈도우 사이즈 변결될 때 마다 호출해 주어야 함)
    RECT rc;
    GetClientRect(hwnd, &rc);
    renderer->set_window_size(rc.right, rc.bottom);


    typedef Ref<GeomHeadNode> RefHeadNode;
    typedef Ref<GeomCurveNode> RefCurveNode;
    typedef Ref<GeomCircleNode> RefCircleNode;
    typedef Ref<GeomRectNode> RefRectNode;
    typedef Ref<GeomCoverLineNode> RefCoverLineNode;
    typedef Ref<GeomSubtractNode> RefSubtractNode;
    typedef Ref<GeomCloneToNode> RefCloneTo;
    typedef Ref<GeomCloneFromNode> RefCloneFrom;
    typedef Ref<GeomRotateNode> RefRotateNode;
    typedef Ref<GeomMoveNode> ReMoveNode;
    typedef Ref<Expression> RefExpression;
    typedef Ref<CSNode> RefCSNode;

    Kernel* kernel = Kernel::instance();

//     // Circle / CloneTo / Subtract
//     RefHeadNode head1       = kernel->create("GeomHeadNode",      "/usr3/geom1");
//     RefCircleNode circle1   = kernel->create("GeomCircleNode",    "/usr3/geom1/CreateCircle");
//     RefCoverLineNode cover1 = kernel->create("GeomCoverLineNode", "/usr3/geom1/CreateCircle/CoverLine");
//     RefCloneTo clone1       = kernel->create("GeomCloneToNode",   "/usr3/geom1/CreateCircle/CoverLine/CloneTo");
//     RefSubtractNode sub1    = kernel->create("GeomSubtractNode",  "/usr3/geom1/CreateCircle/CoverLine/CloneTo/Subtract");
//     RefRotateNode rot1      = kernel->create("GeomRotateNode",    "/usr3/geom1/CreateCircle/CoverLine/CloneTo/Subtract/Rotate");
// 
//     // CloneFrom
//     RefHeadNode head2       = kernel->create("GeomHeadNode",      "/usr3/geom2");
//     RefCloneFrom clone2     = kernel->create("GeomCloneFromNode", "/usr3/geom2/CloneFrom");
//     ReMoveNode move2        = kernel->create("GeomMoveNode",      "/usr3/geom2/CloneFrom/MoveTo");
//     ReMoveNode move21       = kernel->create("GeomMoveNode",      "/usr3/geom2/CloneFrom/MoveTo/MoveTo");
// 
//     // Rectangle 
//     RefHeadNode head3       = kernel->create("GeomHeadNode",      "/usr3/geom3");
//     RefRectNode rect3       = kernel->create("GeomRectNode",      "/usr3/geom3/CreateRectangle");
//     RefCoverLineNode cover3 = kernel->create("GeomCoverLineNode", "/usr3/geom3/CreateRectangle/CoverLine");
// 
//     // CloneFrom 2
//     RefHeadNode head4       = kernel->create("GeomHeadNode",      "/usr3/geom4");
//     RefCloneFrom clone4     = kernel->create("GeomCloneFromNode", "/usr3/geom4/CloneFrom");
//     RefRotateNode rot4      = kernel->create("GeomRotateNode",    "/usr3/geom4/CloneFrom/Rotate");
// 
//     // Cirle5
//     RefHeadNode head5       = kernel->create("GeomHeadNode",      "/usr3/geom5");
//     RefCircleNode circle5   = kernel->create("GeomCircleNode",    "/usr3/geom5/CreateCircle");
//     RefCoverLineNode cover5 = kernel->create("GeomCoverLineNode", "/usr3/geom5/CreateCircle/CoverLine");


    // Circle / CloneTo / Subtract
    RefHeadNode head6       = kernel->create("GeomHeadNode",      "/usr3/geom6");
    RefCircleNode circle6   = kernel->create("GeomCircleNode",    "/usr3/geom6/CreateCircle");
    RefCoverLineNode cover6 = kernel->create("GeomCoverLineNode", "/usr3/geom6/CreateCircle/CoverLine");
    RefSubtractNode sub6    = kernel->create("GeomSubtractNode",  "/usr3/geom6/CreateCircle/CoverLine/Subtract");
    RefRotateNode rot6      = kernel->create("GeomRotateNode",    "/usr3/geom6/CreateCircle/CoverLine/Subtract/Rotate");

    // Cirle7
    RefHeadNode head7       = kernel->create("GeomHeadNode",      "/usr3/geom7");
    RefCircleNode circle7   = kernel->create("GeomCircleNode",    "/usr3/geom7/CreateCircle");
    RefCoverLineNode cover7 = kernel->create("GeomCoverLineNode", "/usr3/geom7/CreateCircle/CoverLine");

    // Curve1
    RefHeadNode head8       = kernel->create("GeomHeadNode",      "/usr3/geom8");
    RefCurveNode curve9     = kernel->create("GeomCurveNode",     "/usr3/geom8/CreateCurve");

    // Coordinate system for circle1
    RefCSNode cs1           = kernel->create("CSNode",        "/cs/cs1");
    RefCSNode cs2           = kernel->create("CSNode",        "/cs/cs1/cs2");
    RefCSNode cs3           = kernel->create("CSNode",        "/cs/cs3");

    cs1->setParameters("0, 0", "0");
    cs2->setParameters("0, 0", "_pi/3");


//     circle1->setParameters("0, 0", "120");
//     circle1->setParameters("11.25, 0", "100");
//     rot1->setParameters("_pi/5");
//     sub1->attach(head3);
// 
// 
//     move2->setParameters("-30", "-20");
//     move21->setParameters("30", "10");
//     clone2->setReferenceNode(clone1);
// 
//     rect3->setParameters("-30, -30", "60", "60");
// 
//     head1->setColor(Color(255, 0, 0, 200));
//     head2->setColor(Color(0, 255, 0, 200));
//     head3->setColor(Color(0, 0, 255, 200));
// 
// 
//     clone4->setReferenceNode(clone1);
//     rot4->setAngle("_pi/3");
//     rot4->setReferedCS(cs1);
// 
//     circle5->setParameters("0, 0", "60");

    circle6->setParameters("0, 0", "100");
    circle7->setParameters("0, 0", "90");
    sub6->attach(head7);
    rot6->setAngle("_pi/4");

    circle7->setParameters("0, 0", "90");

    curve9->setParameters("0, 0", "0,6");


//     //circle1->setReferedCS(cs1);
//     rect3->setReferedCS(cs2);
//     circle1->setReferedCS(nullptr);
// 
//     cs1->setParameters("10, 20", "_pi/3");
//     cs2->setParameters("0, 0", "_pi/5");
// 
//     // detach/ re-attach 시 update() 확인
//     circle1->detach();
//     head1->attach(circle1);


    GeomToPolyTriangle* conv = GeomToPolyTriangle::instance();
    conv->makePolyStructures("/usr3");
    const PolyHolesHelper* helper = conv->getPolyHoles(0);
    

    renderer->set_model_rect(-100, -100, 100, 100);
    renderer->fit_to_window();

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        renderer->draw();
        Sleep(0);

        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyWindow(hwnd);
}

//-----------------------------------------------------------------------------
void GraphicsTestCase::test_MeshInput()
{
    HWND hwnd = CreateDialog(
        GetModuleHandle(0),
        MAKEINTRESOURCE(IDD_RENDERTESTDIALOG),
        0,
        DialogProc);
    ShowWindow(hwnd, SW_NORMAL);


    typedef Ref<ModelerView> RefRenderer;
    RefRenderer renderer = Kernel::instance()->create(
        "ModelerView", "/sys/server/renderer");

    // 렌더링뷰의 윈도우 핸들 설정
    renderer->set_window_handle(int(hwnd));

    // 렌더링뷰의 실제 사이즈 설정 (윈도우 사이즈 변결될 때 마다 호출해 주어야 함)
    RECT rc;
    GetClientRect(hwnd, &rc);
    renderer->set_window_size(rc.right, rc.bottom);


    typedef Ref<GeomHeadNode> RefHeadNode;
    typedef Ref<GeomCircleNode> RefCircleNode;
    typedef Ref<GeomRectNode> RefRectNode;
    typedef Ref<GeomCurveNode> RefCurveNode;
    typedef Ref<GeomCoverLineNode> RefCoverLineNode;
    typedef Ref<GeomSubtractNode> RefSubtractNode;
    typedef Ref<GeomCloneToNode> RefCloneTo;
    typedef Ref<GeomCloneFromNode> RefCloneFrom;
    typedef Ref<GeomRotateNode> RefRotateNode;
    typedef Ref<GeomMoveNode> ReMoveNode;
    typedef Ref<Expression> RefExpression;
    typedef Ref<CSNode> RefCSNode;

    Kernel* kernel = Kernel::instance();


    // Rectangle 
    RefHeadNode head1       = kernel->create("GeomHeadNode",      "/usr5/geom1");
    RefRectNode rect1       = kernel->create("GeomRectNode",      "/usr5/geom1/CreateRectangle");
    RefCoverLineNode cover1 = kernel->create("GeomCoverLineNode", "/usr5/geom1/CreateRectangle/CoverLine");
    RefSubtractNode sub1    = kernel->create("GeomSubtractNode",  "/usr5/geom1/CreateRectangle/CoverLine/Subtract");

    // Curve1
    RefHeadNode head2       = kernel->create("GeomHeadNode",      "/usr5/geom2");
    RefCurveNode curve2     = kernel->create("GeomCurveNode",     "/usr5/geom2/CreateCurve");

    RefHeadNode head3       = kernel->create("GeomHeadNode",      "/usr5/geom3");
    RefCircleNode circle3   = kernel->create("GeomCircleNode",    "/usr5/geom3/CreateCircle");
    RefRotateNode rotate3   = kernel->create("GeomRotateNode",    "/usr5/geom3/CreateCircle/Rotate");
    RefCoverLineNode cover3 = kernel->create("GeomCoverLineNode", "/usr5/geom3/CreateCircle/Rotate/CoverLine");



    // CS1
    RefCSNode cs1           = kernel->create("CSNode",            "/cs/cs1");
    

    rect1->setParameters("-10, -10", "20", "20");
    curve2->setParameters("0, 0", "0, 6");
    circle3->setParameters("0, 0", "10.00001");
    rotate3->setAngle("_pi/10");
    cs1->setParameters("0, 0", "_pi/2");
    sub1->attach(head3);
    rect1->setReferedCS(cs1);


    curve2->pointOnCurve(0, 4);

    GeomToPolyTriangle* conv = GeomToPolyTriangle::instance();
    conv->makePolyStructures("/usr5");

    std::list<Vector2> result;
    conv->getPointsOnHoles(result);

    // 렌더링뷰의 렌더링 경로 설정
    renderer->set_geometry_path("/usr5");
    renderer->set_model_rect(-15, 15, 15, -15);
    renderer->fit_to_window();

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
        renderer->draw();
        Sleep(0);

        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyWindow(hwnd);
}
