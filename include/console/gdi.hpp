
/*!
 * \brief C++ console graphic framework
 * \file  gdi.hpp
 */

#if !defined(GDI_H__)
#define GDI_H__

#include "console.hpp"
#include <algorithm>

namespace xtd {

    template <
        typename T
        >
    struct gdi {
        using console_type = T;
//        using char_type = typename console_type::char_type;

    private:
        console_type& con_;

    public:
        gdi(console_type& con) :
            con_(con)
        {
        }

        /*!
         * \brief line
         * \param src
         * \param dst
         * \param c
         */
        void line(
            coord_type src,
            coord_type dst,
            char_type c = ' '
            )
        {
            short_type x, y;
            const short_type dx = dst.X - src.X;
            const short_type dy = dst.Y - src.Y;
            const short_type dx1 = abs(dx);
            const short_type dy1 = abs(dy);

            if (dy1 <= dx1) {
                // the line is X-axis dominant
                // calculate error intervals for x axis
                short_type px = 2 * dy1 - dx1;
                short_type xmax = std::max(src.X, dst.X);

                if (dx >= 0) {
                    // line is drawn left to right
                    x = src.X;
                    y = src.Y;
                } else {
                    // line is drawn right to left (swap ends)
                    x = dst.X;
                    y = dst.Y;
                }

                con_.move_cursor(make_coord(x, y));
                con_.print(c/*, col*/);
                for (auto i = 0; x < xmax; ++i) {
                    x = x + 1;
                    if (px < 0) {
                        px = px + 2 * dy1;
                    } else {
                        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                            y = y + 1;
                        } else {
                            y = y - 1;
                        }

                        px = px + 2 * (dy1 - dx1);
                    }

                    con_.move_cursor(make_coord(x, y));
                    con_.print(c/*, col*/);
                }
            } else {
                // the line is y-axis dominant
                // calculate error intervals for y axis
                short_type py = 2 * dx1 - dy1;
                short_type ymax = std::max(src.Y, dst.Y);

                if (dy >= 0) {
                    // line is drawn bottom to top
                    x = src.X;
                    y = src.Y;
                } else {
                    // line is drawn top to bottom
                    x = dst.X;
                    y = dst.Y;
                }

                con_.move_cursor(make_coord(x, y));
                con_.print(c/*, col*/);
                for (auto i = 0; y < ymax; ++i) {
                    y = y + 1;
                    if (py <= 0) {
                        py = py + 2 * dx1;
                    } else {
                        if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                            x = x + 1;
                        } else {
                            x = x - 1;
                        }

                        py = py + 2 * (dx1 - dy1);
                    }

                    con_.move_cursor(make_coord(x, y));
                    con_.print(c/*, col*/);
                }
            }
        }

        void rectangle(
            coord_type src,
            coord_type dst,
            char_type c = ' '
            )
        {
            line(
                make_coord(src.X, src.Y),
                make_coord(dst.X, src.Y),
                c
                );
            line(
                make_coord(dst.X, src.Y),
                make_coord(dst.X, dst.Y),
                c
                );
            line(
                make_coord(dst.X, dst.Y),
                make_coord(src.X, dst.Y),
                c
                );
            line(
                make_coord(src.X, dst.Y),
                make_coord(src.X, src.Y),
                c
                );
        }

        void fill(
            coord_type src,
            coord_type dst,
            char_type c = ' '
            )
        {
            for (auto y = 0; y < (dst.Y - src.Y + 1); ++y) {
                    con_.move_cursor(make_coord(src.X, src.Y + y));
                for (auto x = 0; x < (dst.X - src.X + 1); ++x) {
                    con_.print(c);
                }
            }
        }

    };


} // namespace xtd

#endif // GDI_H__
