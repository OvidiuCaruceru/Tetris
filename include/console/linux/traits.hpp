
/*!
 * \brief C++ console framework
 * \file  console.hpp
 */

#if !defined(LINUX_TRAITS_H__)
#define LINUX_TRAITS_H__

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
        using char_type = char;
        using handle_type = int;
        using char_info = int;
        using short_type = short;
        using coord_type = struct coord;
        using rec_type = struct rect;
        using attr_type = short;
        using ulong_t = unsigned long;
    };


} // namespace xtd

#endif // LINUX_TRAITS_H__
