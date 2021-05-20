
/*!
 * \brief C++ console framework
 * \file  buffer.h
 */

#if !defined(BUFFER_H__)
#define BUFFER_H__

#include "console/types.hpp"
#include "attribute.hpp"
#include "exception.hpp"
#include <streambuf>
#include <memory>
#include <algorithm>
#include <cassert>

namespace xtd {

    /*!
     * \brief buffer structure
     */
    template <typename T>
    struct buffer {
        using char_type = T;
        using color = xtd::color;

    private:
        short_type width_;      // number of colums
        short_type height_;     // number of lines
        std::unique_ptr<char_info[]> cells_;
        coord_type cursor_;
        attr::value_type attr_;
        rec_type rframe_;			// refresh frame

        ///
        /// \brief make_color
        /// \param fg
        /// \param bg
        /// \return
        ///
        attr_type make_color(color fg, color bg) {
            attr c;
            c.set_color(fg);
            c.set_background(bg);
            return c.get_attr();
        }

    public:
        explicit buffer(coord_type pos) :
            width_(pos.X),
            height_(pos.Y),
            cursor_(coord_type{0, 0}),
            attr_(make_color(color::white, color::black)),
            rframe_(make_rec(0, 0, 1, 1))
        {
            cells_ = std::make_unique<char_info[]>(width_ * height_);
            assert(nullptr != cells_);
        }

        ~buffer() noexcept = default;

        /*!
         * \brief get buffer width (number of columns)
         * \return
         */
        short_type width() const noexcept {
            return width_;
        }

        /*!
         * \brief get buffer height (number of lines)
         * \return
         */
        short_type height() const noexcept {
            return height_;
        }

        rec_type refresh_frame() const noexcept {
            return rframe_;
        }

        void reset_refresh_frame() {
            rframe_ = make_rec(0, 0, 1, 1);
        }

//        short_type coord2index(coord_type pos) const noexcept {
        int coord2index(coord_type pos) const noexcept {
            return (pos.Y * width() + pos.X);
        }

        coord_type index2coord(short_type index) const noexcept {
            short_type x = index - (index / width()) * width();
            short_type y = index / width();
            return coord_type{ x, y };
        }

        /*!
         * \brief operator []
         * \param line
         * \return
         */
        char_info* operator[](short_type line) {
            if (line >= height_) {
                debug_info(L"buffer::operator[]()");
                // throw exception
                assert(false);
            }

            return (cells_.get() + line * width_);
        }

        /*!
         * \brief cells
         * \return
         */
        char_info* cells() const noexcept {
            return cells_.get();
        }

        /*!
         * \brief get buffer size, i.e. width and height
         * \return
         */
        coord_type size() const noexcept {
            return coord_type{ width(), height() };
        }

        ///
        /// \brief set_color
        /// \param c
        ///
        void set_color(color c) noexcept {
            attr tmp(attr_);
            tmp.set_color(c);
            attr_ = tmp.get_attr();
        }

        ///
        /// \brief set_background
        /// \param c
        ///
        void set_background(color c) noexcept {
            attr tmp(attr_);
            tmp.set_background(c);
            attr_ = tmp.get_attr();
        }

        /*!
         * \brief clear
         */
        void clear_area(
            coord_type top_left,
            coord_type bottom_right,
            color fg = color::white,
            color bg = color::black
            ) noexcept
        {
            coord_type tmp = cursor_;
            attr_type attr = make_color(fg, bg);
            fill(
                top_left,
                bottom_right,
                L' ',
                attr
                );
            cursor_ = tmp;

            // adjust refresh frame
            update_refresh(top_left, bottom_right);
        }

        /*!
         * \brief clear
         */
        void clear(
            color fg = color::white,
            color bg = color::black
            ) noexcept
        {
            clear_area(
                make_coord(0, 0),
                make_coord(width() - 1, height() - 1),
                fg,
                bg
                );
            cursor_.X = 0;
            cursor_.Y = 0;
        }

        /*!
         * \brief valid_point
         * \param pos
         * \return
         */
        bool valid_point(coord_type pos) const noexcept {
            return (
                (pos.X >= 0) && (pos.X < width()) &&
                (pos.Y >= 0) && (pos.Y < height())
                );
        }

        ///
        /// \brief retrieve cursor position
        /// \return
        ///
        coord_type cursor() const noexcept {
            return cursor_;
        }

        ///
        /// \brief move cursor to a new position
        /// \param pos
        ///
        void move_cursor(coord_type pos) {
            cursor_ = pos;
        }

        /*!
         * \brief draw one pixel
         * \param c    Character to be printed
         * \param attr Console attributes
         *
         * \note
         * If 'attr' is zero, the active attributes will be used.
         */
        std::size_t print(
            char_type c,
            attr_type attr = 0
            )
        {
            if (!valid_point(cursor_)) {
                assert(false);
                return 0;
            }

            auto i = coord2index(cursor_);
            cells_[i].Char.UnicodeChar = (wchar_t)c;
            cells_[i].Attributes = (0 == attr) ? attr_ : attr;

            // adjust refresh frame
            update_refresh(
                make_coord(cursor_.X, cursor_.Y),
                make_coord(cursor_.X + 1, cursor_.Y)
                );
            ++cursor_.X;

            return 1;
        }

        /*!
         * \brief print
         * \param s    String to be printed
         * \param attr Console attributes
         * \return Number of characters printed
         *
         * \note
         * If 'attr' is zero, the active attributes will be used.
         */
        std::size_t print(
            const char_type* s,
            attr_type attr = 0
            )
        {
            coord_type pos = cursor_;
            std::size_t count = 0;
            for ( ; (wchar_t)s[count] != L'\0'; ++count) {
                if (cursor_.X >= width() - 1) {
                    break;
                }

                if (L'\n' == (wchar_t)s[count]) {
                    cursor_.X = 0;
                    ++cursor_.Y;
                    continue;
                }

                auto idx = coord2index(cursor_);
                cells_[idx].Char.UnicodeChar = (wchar_t)s[count];
//                cells_[idx].Char.AsciiChar = s[i];
                cells_[idx].Attributes = (0 == attr) ? attr_ : attr;
                ++cursor_.X;
            }

            update_refresh(
                make_coord(pos.X, pos.Y),
                make_coord(pos.X + count, pos.Y)
                );
            return count;
        }

        /*!
         * \brief fill
         * \param src
         * \param dst
         * \param c
         * \param attr
         */
        void fill(
            coord_type src,
            coord_type dst,
            char_type c /*= 0x2588*/,
            attr_type attr //= 0x000F
            )
        {
            coord_type pos = cursor_;
            for (int x = src.X; x < dst.X; ++x) {
                for (int y = src.Y; y < dst.Y; ++y) {
                    move_cursor(make_coord(x, y));
                    print(/*make_coord(x, y),*/ c, attr);
                }
            }

            update_refresh(src, dst);
            move_cursor(pos);
        }

    private:
        void update_refresh(
            coord_type tl,
            coord_type br
            )
        {
            rframe_.Left = std::min(rframe_.Left, tl.X);
            rframe_.Top = std::min(rframe_.Top, tl.Y);
            rframe_.Right = std::max(rframe_.Right, br.X);
            rframe_.Bottom = std::max(rframe_.Bottom, br.Y);
        }
    };


    ///
    /// \brief redirect std::cout to a different buffer
    /// \note
    /// https://accu.org/index.php/journals/264
    ///
    template<
        typename T,
        typename traits = std::char_traits<T>
        >
    struct streambuf :
        public std::basic_streambuf<T, traits>
    {
    private:
        using char_type = typename traits::char_type;
        using int_type = typename std::basic_streambuf<T, traits>::int_type;
        using buffer_type = buffer<char_type>;

        buffer_type& buff_;

    public:
        streambuf(buffer_type& buff) :
            buff_(buff)
        {
        }

    protected:
        ///
        /// \brief overflow
        /// \param c
        /// \return
        ///
        int_type overflow(int_type c = traits::eof()) override {
            // check character is not EOF
            if (!traits::eq_int_type(c, traits::eof())) {
                buff_.print(/*pos,*/ c);
            }

            return traits::not_eof(c);
        }

        ///
        /// \brief xsputn
        /// \param s
        /// \param count
        /// \return
        ///
        std::streamsize xsputn(const char_type* s, std::streamsize /*count*/) override {
            return buff_.print(/*pos,*/ s);
        }
    };

} // namespace xtd

#endif // BUFFER_H__
