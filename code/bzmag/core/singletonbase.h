#ifndef BZMAG_CORE_UTILITY_SINGLETONBASE_H
#define BZMAG_CORE_UTILITY_SINGLETONBASE_H
/**
    @ingroup bzmagCoreUtility
    @class bzmag::SingletonBase
    @brief 
*/

#include "singletonserver.h"

namespace bzmag
{
    class SingletonBase
    {
    public:
        SingletonBase():refCount_(0) {}
        virtual ~SingletonBase() {}

        virtual int addRefSingleton();
        virtual int releaseSingleton();
        virtual int getRefSingleton() const;

    protected:
        int refCount_;
    };

#include "singletonbase.inl"

}

#endif // BZMAG_CORE_UTILITY_SINGLETONBASE_H
