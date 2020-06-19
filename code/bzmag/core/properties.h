#ifndef BZMAG_CORE_PROPERTY_PROPERTIES_H
#define BZMAG_CORE_PROPERTY_PROPERTIES_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Properties
    @brief
*/

#include <map>
#include "property.h"

namespace bzmag
{
    class Properties : public std::map<String, Property*>
    {
    public:
    };
}

#endif // BZMAG_CORE_PROPERTY_PROPERTIES_H
