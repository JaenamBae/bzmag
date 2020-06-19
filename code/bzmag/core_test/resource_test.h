#ifndef bzmag_CORETEST_RESOURCETESTCASE_H
#define bzmag_CORETEST_RESOURCETESTCASE_H

#include "core/unittest.h"

class ResourceTestCase : public bzmag::TestCase<ResourceTestCase>
{
public:
    BZMAGUNIT_BEGINE_TESTSUITE(ResourceTestCase)
        BZMAGUNIT_TEST(test_Uri_split)
        BZMAGUNIT_TEST(test_ResourceManager_addResource)
        BZMAGUNIT_TEST(test_ResourceManager_removeResource)
        BZMAGUNIT_TEST(test_ResourceManager_findResource)
    BZMAGUNIT_END_TESTSUITE()

    void test_Uri_split();
    void test_ResourceManager_addResource();
    void test_ResourceManager_removeResource();
    void test_ResourceManager_findResource();
};

#endif // bzmag_CORETEST_RESOURCETESTCASE_H
