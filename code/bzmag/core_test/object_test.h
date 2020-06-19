#ifndef bzmag_CORETEST_OBJECTTESTCASE_H
#define bzmag_CORETEST_OBJECTTESTCASE_H

#include "core/unittest.h"

class ObjectTestCase : public bzmag::TestCase<ObjectTestCase>
{
public:
    BZMAGUNIT_BEGINE_TESTSUITE(ObjectTestCase)
        BZMAGUNIT_TEST(test_Object)
        BZMAGUNIT_TEST(test_CreateObjectByConcreteType)
        BZMAGUNIT_TEST(test_PrimitiveTypeProperty)
        BZMAGUNIT_TEST(test_CustomPrimitiveTypeProperty)
        BZMAGUNIT_TEST(test_StructTypeProperty)
        BZMAGUNIT_TEST(test_EnumTypeProperty)
        BZMAGUNIT_TEST(test_ListTypeProperty)
        BZMAGUNIT_TEST(test_DictionaryTypeProperty)
        BZMAGUNIT_TEST(test_NodeTypeProperty)
        BZMAGUNIT_TEST(test_ReflectionMethod)
        BZMAGUNIT_TEST(test_Node)
        BZMAGUNIT_TEST(test_AbstractNode)
    BZMAGUNIT_END_TESTSUITE()

    void test_Object();
    void test_CreateObjectByConcreteType();
    void test_PrimitiveTypeProperty();
    void test_CustomPrimitiveTypeProperty();
    void test_StructTypeProperty();
    void test_EnumTypeProperty();
    void test_ListTypeProperty();
    void test_DictionaryTypeProperty();
    void test_NodeTypeProperty();
    void test_ReflectionMethod();
    void test_Node();
    void test_AbstractNode();
    //void test_LinkNode();
};

#endif // bzmag_CORETEST_OBJECTTESTCASE_H
