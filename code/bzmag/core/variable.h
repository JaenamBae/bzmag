#ifndef BZMAG_CORE_OBJECT_VARIABLE_H
#define BZMAG_CORE_OBJECT_VARIABLE_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Variable
    @brief
*/

#include "primitivetype.h"

namespace bzmag
{
    class Variable
    {
    public:
        virtual ~Variable() {}
        virtual Variable* clone()=0;
        virtual type_id getType() const=0;
    };
}

#endif // BZMAG_CORE_OBJECT_VARIABLE_H
