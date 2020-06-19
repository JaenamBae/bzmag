#include "core_test/utility_test.h"


#include "core/string.h"
#include "core/enumeration.h"

using namespace bzmag;

//-----------------------------------------------------------------------------
void UtilityTestCase::test_String()
{
    String s1;
    s1.format("%d %.1f %s", 3, 0.3, "TEST");
    BZMAGUNIT_ASSERT(s1 == "3 0.3 TEST");

    String s2("%d %.1f %s", 3, 0.3, "TEST");
    BZMAGUNIT_ASSERT(s2 == "3 0.3 TEST");

    // Unicode Encoding
    String u1(L"test_¹èÀç³²_ª³ªóªËªÁ(ÐÑìí)ªÏ");
    BZMAGUNIT_ASSERT(u1 == "test_¹èÀç³²_ª³ªóªËªÁ(ÐÑìí)ªÏ");

    String u2;
    u2.format(L"test %d %d", 2, 3);
    BZMAGUNIT_ASSERT(u2 == "test 2 3");



}

//-----------------------------------------------------------------------------
class TestEnum
{
public:
    enum Value
    {
        TESTENUM_1 = 1,
        TESTENUM_2 = 100,
        TESTENUM_3,
        TESTENUM_INVALID = -1,
        TESTENUM_MAX,
    };
};

typedef Enumeration<TestEnum::Value, TestEnum::TESTENUM_MAX, TestEnum::TESTENUM_INVALID> TestEnumeration;
struct TestEnumInitializer : public TestEnumeration
{
    static void initialize()
    {
        add("TestEnum::TESTENUM_1", TestEnum::TESTENUM_1);
        add("TestEnum::TESTENUM_2", TestEnum::TESTENUM_2);
        add("TestEnum::TESTENUM_3", TestEnum::TESTENUM_3);
    }
};

void UtilityTestCase::test_Enumeration()
{
    TestEnumInitializer::initialize();
    TestEnumeration te;

    BZMAGUNIT_ASSERT(sizeof(TestEnumeration) == 4);

    // assign operator
    te = TestEnum::TESTENUM_1;
    BZMAGUNIT_ASSERT(te == TestEnum::TESTENUM_1);
    te = TestEnum::TESTENUM_2;
    BZMAGUNIT_ASSERT(te == TestEnum::TESTENUM_2);

    // copy constructor
    TestEnumeration te2(te);
    BZMAGUNIT_ASSERT(te2 == TestEnum::TESTENUM_2);
    BZMAGUNIT_ASSERT(te == te2);
    TestEnumeration te3(TestEnum::TESTENUM_3);
    BZMAGUNIT_ASSERT(te3 == TestEnum::TESTENUM_3);
    TestEnumeration te4(100);
    BZMAGUNIT_ASSERT(te4 == TestEnum::TESTENUM_2);

    te2 = te3;
    BZMAGUNIT_ASSERT(te2 == TestEnum::TESTENUM_3);

    // from string
    TestEnumeration te5("TestEnum::TESTENUM_1");
    BZMAGUNIT_ASSERT(te5 == TestEnum::TESTENUM_1);
    te5 = "TestEnum::TESTENUM_2";
    BZMAGUNIT_ASSERT(te5 == TestEnum::TESTENUM_2);
    te5 = "TestEnum::TESTENUM_4";
    BZMAGUNIT_ASSERT(te5 == TestEnum::TESTENUM_INVALID);

    // to string
    TestEnumeration te6(TestEnum::TESTENUM_1);
    BZMAGUNIT_ASSERT(te6 == "TestEnum::TESTENUM_1");
    te6 = TestEnum::TESTENUM_2;
    BZMAGUNIT_ASSERT(te6 == "TestEnum::TESTENUM_2");

    // enumerator
    TestEnum::Value a[] =
    {
        TestEnum::TESTENUM_1,
        TestEnum::TESTENUM_2,
        TestEnum::TESTENUM_3,
    };
    int i = 0;
    for (TestEnumeration::enumerator e = te6.beginEnum();
         e != te6.endEnum(); ++e, ++i)
         BZMAGUNIT_ASSERT(e->first == a[i]);
}
