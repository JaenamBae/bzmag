#ifndef BZMAG_CORE_OBJECT_OBJECT_H
#define BZMAG_CORE_OBJECT_OBJECT_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Object
    @brief
*/

#include "primitivetype.h"
#include "String.h"
#include "type.h"

namespace bzmag
{
    class Object
    {
    public:
        Object();
        virtual ~Object();
        DECLARE_SUPERCLASS(Object);
        template<class T> friend class Ref;
        template<class T> friend class Singleton3;

    public:
        uint32 getRef() const;
        uint32 getID() const;
        bool isKindOf(const String& name);
        bool isKindOf(const Type* type);

    public:
        static void bindMethod();
        static void bindProperty();

    protected:
        void autorelease();
        uint32 addRef();
        uint32 release();

    private:
        uint32 ID_;
        uint32 refCount_;

    private:
        static uint32 sID_;
    };
}

#endif // BZMAG_CORE_OBJECT_OBJECT_H
