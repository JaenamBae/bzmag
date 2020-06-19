#include "bzmag/core/unittest.h"
#include "bzmag/core/kernel.h"
#include "bzmag/core/module.h"
/*#include "bzmag/core/resourcemanager.h"*/
#include "bzmag/engine_test/graphics_test.h"
#include <windows.h>

using namespace bzmag;

INCLUDE_MODULE(Engine);
INCLUDE_MODULE(WinGraphics)

//-----------------------------------------------------------------------------
int APIENTRY WinMain
(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance,
    __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    //	_CrtSetBreakAlloc(295);

    USING_MODULE(Engine);
    USING_MODULE(WinGraphics)

    bzmag::TestSuite test_suite;
    test_suite.addTestCase<GraphicsTestCase>();
    test_suite.run();

    return 0;
}
