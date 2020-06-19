#ifndef BZMAG_CORE_KERNEL_BUILTINMODULE_H
#define BZMAG_CORE_KERNEL_BUILTINMODULE_H
/**
    @ingroup bzmagCoreKernel
    @class bzmag::BuiltinModule
    @brief 
*/

#include "module.h"

namespace bzmag
{
    class bzmag_;
    class Object;
    class BuiltinModule : public Module
    {
    public:
        BuiltinModule(Kernel* kernel, const String& name);
        virtual~BuiltinModule();
    };
}

#endif // BZMAG_CORE_KERNEL_BUILTINMODULE_H
