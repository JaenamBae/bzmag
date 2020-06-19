#ifndef BZMAG_CORE_OBJECT_PARAMETER_H
#define BZMAG_CORE_OBJECT_PARAMETER_H
/**
    @ingroup bzmagCoreObject
    @class bzmag::Parameter
    @brief
*/

#include "variables.h"

namespace bzmag
{
    class Parameter
    {
    public:
        //virtual ~Parameter() {}
        void clear();

        Variables* in();
        Variables* out();

    private:
        Variables in_;
        Variables out_;
    };

#include "parameter.inl"

}

#endif // BZMAG_CORE_OBJECT_PARAMETER_H
