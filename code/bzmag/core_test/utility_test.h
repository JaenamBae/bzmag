#ifndef bzmag_CORETEST_UTILITYTESTCASE_H
#define bzmag_CORETEST_UTILITYTESTCASE_H

#include "core/unittest.h"

class UtilityTestCase : public bzmag::TestCase<UtilityTestCase>
{
public:
    BZMAGUNIT_BEGINE_TESTSUITE(UtilityTestCase)
        BZMAGUNIT_TEST(test_String);
        BZMAGUNIT_TEST(test_Enumeration);
    BZMAGUNIT_END_TESTSUITE()

    void test_String();
    void test_Enumeration();
};

#endif // bzmag_CORETEST_UTILITYTESTCASE_H
