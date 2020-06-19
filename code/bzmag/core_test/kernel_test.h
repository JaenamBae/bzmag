#ifndef bzmag_CORETEST_KERNALTESTCASE_H
#define bzmag_CORETEST_KERNALTESTCASE_H

#include "core/unittest.h"

class KernelTestCase : public bzmag::TestCase<KernelTestCase>
{
public:
    BZMAGUNIT_BEGINE_TESTSUITE(KernelTestCase)
        BZMAGUNIT_TEST(test_Path);
        BZMAGUNIT_TEST(test_CreateObject)
        BZMAGUNIT_TEST(test_Lookup)
    BZMAGUNIT_END_TESTSUITE()

    void test_Path();
    void test_CreateObject();
    void test_Lookup();
};

#endif // bzmag_CORETEST_KERNALTESTCASE_H
