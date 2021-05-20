
/*!
 * \file  attribute.hpp
 * \brief
 */

#if !defined(WIN_CONSOLE_H__)
#define WIN_CONSOLE_H__

#include "console/types.hpp"
#include "exception.hpp"
#include <windows.h>
#include <io.h>
#include <cassert>


namespace xtd {

    ///
    /// \brief generic color code(s)
    ///
    enum class color {
        black,
        darkblue,
        darkgreen,
        darkcyan,
        darkred,
        darkmagenta,
        darkyellow,
        darkgray,
        gray,
        blue,
        green,
        cyan,
        red,
        magenta,
        yellow,
        white,
    };

    ///
    /// \brief attr struct
    ///
    struct attr {
        using value_type = SHORT;
        using color_type = xtd::color;

    private:
//        color_type fcolor_;		// forground color
//        color_type bcolor_;		// background color
        value_type attr_;

    public:
        attr(value_type value) :
            attr_(value)
        {
        }

        attr(
            color_type fcolor = color_type::white,
            color_type bcolor = color_type::black
            )
        {
            set_color(fcolor);
            set_background(bcolor);
        }

        ///
        /// \brief set console foreground color
        /// \param c Color to be set
        ///
        void set_color(color_type c) noexcept {
            static const int colors[] = {
                /* BLACK */			0,
                /* DARKBLUE */		FOREGROUND_BLUE,
                /* DARKGREEN */		FOREGROUND_GREEN,
                /* DARKCYAN */		FOREGROUND_GREEN | FOREGROUND_BLUE,
                /* DARKRED */		FOREGROUND_RED,
                /* DARKMAGENTA */	FOREGROUND_RED | FOREGROUND_BLUE,
                /* DARKYELLOW */		FOREGROUND_RED | FOREGROUND_GREEN,
                /* DARKGRAY */		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
                /* GRAY */			FOREGROUND_INTENSITY,
                /* BLUE */			FOREGROUND_INTENSITY | FOREGROUND_BLUE,
                /* GREEN */			FOREGROUND_INTENSITY | FOREGROUND_GREEN,
                /* CYAN */			FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
                /* RED */			FOREGROUND_INTENSITY | FOREGROUND_RED,
                /* MAGENTA */		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
                /* YELLOW */		FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
                /* WHITE */			FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
            };
            std::size_t idx = (std::size_t)c;
            if (idx < sizeof(colors)) {
                attr_ &= 0xF0;
                attr_ |= colors[idx];
            }
        }

        ///
        /// \brief set console background color
        /// \param c Color to be set
        ///
        void set_background(color_type c) noexcept {
            static const int bcolors[] = {
                /* BLACK */			0,
                /* DARKBLUE */		BACKGROUND_BLUE,
                /* DARKGREEN */		BACKGROUND_GREEN,
                /* DARKCYAN */		BACKGROUND_GREEN | BACKGROUND_BLUE,
                /* DARKRED */		BACKGROUND_RED,
                /* DARKMAGENTA */	BACKGROUND_RED | BACKGROUND_BLUE,
                /* DARKYELLOW */		BACKGROUND_RED | BACKGROUND_GREEN,
                /* DARKGRAY */		BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
                /* GRAY */			BACKGROUND_INTENSITY,
                /* BLUE */			BACKGROUND_INTENSITY | BACKGROUND_BLUE,
                /* GREEN */			BACKGROUND_INTENSITY | BACKGROUND_GREEN,
                /* CYAN */			BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
                /* RED */			BACKGROUND_INTENSITY | BACKGROUND_RED,
                /* MAGENTA */		BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
                /* YELLOW */		BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
                /* WHITE */			BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
            };
            std::size_t idx = (std::size_t)c;
            if (idx < sizeof(bcolors)) {
                attr_ &= 0x0F;
                attr_ |= bcolors[idx];
            }
        }

        ///
        /// \brief get_attr
        /// \return
        ///
        value_type get_attr() const noexcept {
            return attr_;
        }

        ///
        /// \brief set_attr
        /// \param value
        ///
        void set_attr(value_type value) noexcept {
            attr_ = value;
        }
    };


    ///
    /// \brief console_attr struct
    ///
    struct console_attr {
        using color_type = attr::color_type;

    private:
        using value_type = attr::value_type;

        attr attr_;
        HANDLE h_;
        CONSOLE_SCREEN_BUFFER_INFO csbi_;

    public:
        console_attr() {
//            h_ = CreateFile(
//                L"CONOUT$",
//                GENERIC_READ|GENERIC_WRITE,
//                FILE_SHARE_READ | FILE_SHARE_WRITE,
//                NULL,
//                OPEN_EXISTING,
//                FILE_ATTRIBUTE_NORMAL,
//                NULL
//                );
            h_ = ::GetStdHandle(STD_OUTPUT_HANDLE);
            if (INVALID_HANDLE_VALUE == h_) {
                throw exception("Invalid output handle!");
            }

            ::GetConsoleScreenBufferInfo(h_, &csbi_);
            attr_.set_attr(csbi_.wAttributes);
        }

        ///
        /// \brief set console foreground color
        /// \param c Color to be set
        ///
        void set_color(color_type c) noexcept {
            attr_.set_color(c);
        }

        ///
        /// \brief set console background color
        /// \param c Color to be set
        ///
        void set_background(color_type c) noexcept {
            attr_.set_background(c);
        }

        ///
        /// \brief reset
        ///
        void reset() noexcept {
            attr_.set_attr(csbi_.wAttributes);
        }

        value_type get_attr() const noexcept {
            return attr_.get_attr();
        }

        ///
        /// \brief apply
        ///
        void apply() const {
            ::SetConsoleTextAttribute(h_, attr_.get_attr());
        }
    };

}

#endif // WIN_CONSOLE_H__

