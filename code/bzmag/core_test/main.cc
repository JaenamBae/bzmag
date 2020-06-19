#include "core_test/object_test.h"
#include "core_test/utility_test.h"
#include "core_test/kernel_test.h"
#include "core/testsuite.h"
using namespace bzmag;

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);
    //	_CrtSetBreakAlloc(295);

    bzmag::TestSuite test_suite;

    test_suite.addTestCase<ObjectTestCase>();
    test_suite.addTestCase<UtilityTestCase>();
    test_suite.addTestCase<KernelTestCase>();
    test_suite.run();

    return 0;
}
