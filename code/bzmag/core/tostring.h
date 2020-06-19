#ifndef BZMAG_CORE_UTILITY_TOSTRING_H
#define BZMAG_CORE_UTILITY_TOSTRING_H
/**
    @ingroup bzmagCoreUtility
    @class bzmag::ToString
    @brief 
*/

#include "define.h"
#include "primitivetype.h"
#include "singleton.h"
#include "string.h"
#include <map>

namespace bzmag
{
    class Object;
    class Property;
    class StringConverter;
    class ToString : public Singleton<ToString>
    {
    public:
        ToString();
        virtual~ToString();

        void addConverter(StringConverter* sc);
        const char* getTypeKeyword(const Property* property) const;
        const String& toString(Object* object, Property* property);
        void fromString(Object* object, Property* property, const String& value);

    private:
        typedef std::map<type_id, StringConverter*> StringConverters;

    private:
        StringConverters converters_;
    };
}

#endif // BZMAG_CORE_UTILITY_TOSTRING_H
