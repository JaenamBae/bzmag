#ifndef BZMAG_CORE_PROPERTY_METHODS_H
#define BZMAG_CORE_PROPERTY_METHODS_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Properties
    @brief
*/

#include <map>
#include "method.h"

namespace bzmag
{
    class Methods : public std::map<String, Method*>
    {
    public:
    };
}

#endif // BZMAG_CORE_PROPERTY_METHODS_H
