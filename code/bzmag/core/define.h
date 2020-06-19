#ifndef BZMAG_CORE_BASE_DEFINE_H
#define BZMAG_CORE_BASE_DEFINE_H

#include "platform.h"

//STL helpers
#ifdef BZMAGENGINE_PLATFORM_WIN32
    #include <unordered_map>
    #define bzmag_hash_map std::unordered_map

    #define BZMAG_VSCPRINTF(f, a) vsnprintf(0, 0, f, a)
    #define BZMAG_VSPRINTF vsprintf_s
    #define BZMAG_VSNPRINTF(buf, bufsize, f, a) vsnprintf_s(buf, bufsize, _TRUNCATE, f, a);
    #define BZMAG_VSCWPRINTF(f, a) vsnwprintf_s(0, 0, f, a)
#else
    #include <unordered_map>
    #define bzmag_hash_map std::unordered_map

    #define BZMAG_VSCPRINTF(f, a) vsnprintf(0, 0, f, a)
    #define BZMAG_VSPRINTF vsprintf
    #define BZMAG_VSNPRINTF vsnprintf
#endif 

//directives
#define OVERRIDE virtual
#define IN
#define OUT
#define INOUT

#define ___FILE___ __FILE__
#define ___FUNCSIG___ __FUNCSIG__
#define ___FUNCTION___ __FUNCTION__
#define ___LINE___  __LINE__
#define ___DATE___  __DATE__
#define ___TIME___  __TIME__

//helpers
#define SAFE_DELETE(p) do { if (p) delete p; p = 0; } while (0);
#define SAFE_RELEASE(p) do { if (p) p->Release(); p = 0; } while (0);

//-----------------------------------------------------------------------------
template <typename DERIVED, typename BASE>
DERIVED DOWN_CAST(BASE* p)
{
    return dynamic_cast<DERIVED>(p);
}

#endif // BZMAG_CORE_BASE_DEFINE_H
