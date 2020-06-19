#ifndef BZMAG_CORE_UTILITY_UNITTEST_TESTSUITE_H
#define BZMAG_CORE_UTILITY_UNITTEST_TESTSUITE_H
/**
    @ingroup bzmagCoreUnitTest
    @class bzmag::TestSuite
    @brief 
*/

#include <list>

namespace bzmag
{
    class TestCaseBase;
    class TestSuite
    {
    public:
        virtual~TestSuite();

        template <typename T>
        void addTestCase();
        void run();

    private:
        typedef std::list<TestCaseBase*> TestCases;

    private:
        TestCases testCases_;
    };

#include "testsuite.inl"

}

#endif // BZMAG_CORE_UTILITY_UNITTEST_TESTSUITE_H
