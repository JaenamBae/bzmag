#ifndef BZMAG_CORE_PROPERTY_PROPERTY_H
#define BZMAG_CORE_PROPERTY_PROPERTY_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Property
    @brief
*/

#include "string.h"
#include "primitivetype.h"
#include "typeid.h"

namespace bzmag
{
    class Object;
    class Property
    {
    public:
        void setName(const String& name);
        virtual const String& getName() const;
        const String& toString(Object* object);
        void fromString(Object* object, const String& value);
        const char* getTypeKeyword() const;

        virtual bool isReadOnly() const=0;
        virtual type_id getType() const;
        virtual Property* findProperty(const String& name);

    private:
        String name_;
    };

#include "property.inl"

}

#endif // BZMAG_CORE_PROPERTY_PROPERTY_H
