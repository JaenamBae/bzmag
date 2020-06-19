#ifndef BZMAG_CORE_BASE_PLATFORM_H
#define BZMAG_CORE_BASE_PLATFORM_H

#ifdef WIN32
    #define BZMAGENGINE_PLATFORM_WIN32
    #define WIN32_LEAN_AND_MEAN
    #pragma warning(disable:4244)
#endif

#ifdef __APPLE__
    #define BZMAGENGINE_PLATFORM_UNIX
    #if defined(TARGET_OS_IPHONE) || defined(TARGET_IPHONE_SIMULATOR)
        #define BZMAGENGINE_PLATFORM_IOS
    #elif defined(TARGET_OS_MAC)
        #define BZMAGENGINE_PLATFORM_MAC
    #endif
    #define sealed
#endif

#if defined(__GNUC__) || defined(__GNUG__)
    #define BZMAGENGINE_PLATFORM_UNIX
#endif


#endif // BZMAG_CORE_BASE_PLATFORM_H
