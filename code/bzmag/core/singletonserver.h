#ifndef BZMAG_CORE_UTILITY_SINGLETONSERVER_H
#define BZMAG_CORE_UTILITY_SINGLETONSERVER_H
/**
@ingroup bzmagCoreUtility
@class bzmag::SingletonServer
@brief
*/

#include <list>

namespace bzmag
{
    class SingletonBase;
    class SingletonServer
    {
    public:
        SingletonServer();
        virtual ~SingletonServer();

        void clear();

        void registerSingleton(SingletonBase* singleton);
        void unregisterSingleton(SingletonBase* singleton);

    public:
        static SingletonServer* instance();

    private:
        static SingletonServer s_instance_;

    private:
        typedef std::list<SingletonBase*> Singletons;

    private:
        Singletons singletons_;
    };
}

#endif // BZMAG_CORE_UTILITY_SINGLETONSERVER_H
