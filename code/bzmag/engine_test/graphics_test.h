#pragma once

#include "core/unittest.h"

class GraphicsTestCase : public bzmag::TestCase<GraphicsTestCase>
{
public:
    BZMAGUNIT_BEGINE_TESTSUITE(GraphicsTestCase)
//        BZMAGUNIT_TEST(test_CGAL);
//        BZMAGUNIT_TEST(test_Renderer);
//        BZMAGUNIT_TEST(test_Boolean);
//        BZMAGUNIT_TEST(test_Band);
        BZMAGUNIT_TEST(test_MeshInput);
    BZMAGUNIT_END_TESTSUITE()

    void test_CGAL();
    void test_Renderer();
    void test_Boolean();
    void test_MeshInput();

};

