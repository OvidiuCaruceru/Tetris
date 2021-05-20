
/*!
 * \file  types.hpp
 * \brief
 */

#if !defined(TYPES_H__)
#define TYPES_H__

/**/
//#define NOMINMAX 
//#include "utils/system.hpp"
#include <cstddef>

//#if defined(OS_LINUX)
//#include "linux/traits.hpp"
//#elif defined(OS_WIN)
//#include "windows/traits.hpp"
//#endif //
#include "traits.hpp"


namespace xtd {

    // type aliases
    using size_type = std::size_t;
    using char_type = xtd::traits::char_type;
    using short_type = traits::short_type;
    using coord_type = traits::coord_type;
    using rec_type = traits::rec_type;
    using attr_type = traits::attr_type;
    using handle_type = traits::handle_type;
    using char_info = traits::char_info;
    using ulong_t = traits::ulong_t;


    /*!
     * \brief make_coord
     * \param x
     * \param y
     * \return
     */
    template <
        typename T1,
        typename T2
        >
    coord_type make_coord(T1 x, T2 y) noexcept {
        return coord_type{
            static_cast<short_type>(x),
            static_cast<short_type>(y)
        };
    }

    /*!
     * \brief make_rec
     * \param x
     * \param y
     * \param dx
     * \param dy
     * \return
     */
    template <
        typename T1,
        typename T2,
        typename T3,
        typename T4
        >
    rec_type make_rec(T1 x, T2 y, T3 dx, T4 dy) {
        return rec_type {
            static_cast<short_type>(x),
            static_cast<short_type>(y),
            static_cast<short_type>(x + dx - 1),
            static_cast<short_type>(y + dy - 1)
        };
    }

} // namespace xtd

#endif //TYPES_H__
