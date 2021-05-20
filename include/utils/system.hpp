
/*!
 * \file  types.hpp
 * \brief
 */

#if !defined(SYSTEM_H__)
#define SYSTEM_H__

/**/
//#define NOMINMAX
#include <cstddef>


//
// OS detection
// (defines the appropriate macro that is used to wrap some
// platform specific things)
//
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    #define OS_WIN
#include <windows.h>
#include <tchar.h>
#elif defined(__unix__) || defined(__unix) || defined(__linux__)
    #define OS_LINUX

//    using short_type = short;
//    using ulong_t = unsigned long;

//    struct coord {
//        short_type x;
//        short_type y;
//    };

//    struct rect {
//        short_type left;
//        short_type top;
//        short_type right;
//        short_type bottom;
//    };

#elif defined(__APPLE__)
    #define OS_MACOS
#else
    #error Unsupported platform!
#endif

#endif //SYSTEM_H__
