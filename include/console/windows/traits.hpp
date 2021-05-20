
/*!
 * \brief C++ console framework
 * \file  console.hpp
 */

#if !defined(WINDOWS_TRAITS_H__)
#define WINDOWS_TRAITS_H__

//#include "console/types.hpp"
//#define NOMINMAX 
#include <windows.h>
#include <tchar.h>

//
// [Note]
// windows.h defines these two macros and they conflicts with
// STL std::min and std::max functions, therefore, we need
// to disable the macros
#undef min
#undef max

namespace xtd {

    /*!
     * \brief The traits struct
     */
    struct traits {
    private:
        struct coord {
            short X;
            short Y;
        };

        struct rect {
            short Left;
            short Top;
            short Right;
            short Bottom;
        };

    public:
        using char_type = wchar_t;
        using handle_type = HANDLE;
        using char_info = CHAR_INFO;
        using short_type = SHORT;
        using coord_type = COORD;
        using rec_type = SMALL_RECT;
        using attr_type = WORD;
        using ulong_t = DWORD;
    };


    /*!
     * \brief font_type structure
     */
    struct font_type {
        wchar_t const* name_;
        traits::short_type width_;
        traits::short_type height_;
    };


    /*!
     * \brief print debug information
     * \param str
     */
    void debug_info(wchar_t const* str) {
        static wchar_t tmp[512];

        wsprintf(tmp, L"%s (error: %d)\n", str, GetLastError());
        OutputDebugString(tmp);
    }

} // namespace xtd

#endif // WINDOWS_TRAITS_H__
