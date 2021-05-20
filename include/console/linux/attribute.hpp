
/*!
 * \file  linux_console.hpp
 * \brief
 */

#if !defined(LINUX_CONSOLE_H__)
#define LINUX_CONSOLE_H__

#include "console_types.hpp"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>


namespace console {

    ///
    /// \brief console_attr struct
    ///
    struct attr {
    private:
        ///
        /// \brief attr_type struct
        ///
        struct attr_type {
            // Linux console color code
            enum class linux_color {
                BLACK       = 0,
                RED         = 1,
                GREEN       = 2,
                YELLOW      = 3,
                BLUE        = 4,
                MAGENTA     = 5,
                CYAN        = 6,
                WHITE       = 7,
            };

            int color_;
            bool bright_;

            attr_type():
                color_(static_cast<int>(linux_color::BLACK)),
                bright_(false)
            {
            }

            ///
            /// \brief reset
            ///
            void reset() {
                color_ = 0;
                bright_ = false;
            }

            ///
            /// \brief convert color to Linux console specific attributes
            /// \param c
            /// \return
            ///
            void set_color(color c, bool foreground = true) noexcept
            {
                struct color_info {
                    //int color_;
                    attr_type::linux_color lcolor_;   // Linux color code
                    bool bright_;
                };
                static const color_info cinfo[] = {
                    { /*color::BLACK,*/ attr_type::linux_color::BLACK, false },
                    { /*color::DARKBLUE,*/ attr_type::linux_color::BLUE, false },
                    { /*color::DARKGREEN,*/ attr_type::linux_color::GREEN, false },
                    { /*color::DARKCYAN,*/ attr_type::linux_color::CYAN, false },
                    { /*color::DARKRED,*/ attr_type::linux_color::RED, false },
                    { /*color::DARKMAGENTA,*/ attr_type::linux_color::MAGENTA, false },
                    { /*color::DARKYELLOW,*/ attr_type::linux_color::YELLOW, false },
                    { /*color::DARKGRAY,*/ attr_type::linux_color::WHITE, true },
                    { /*color::GRAY,*/ attr_type::linux_color::WHITE, true },
                    { /*color::BLUE,*/ attr_type::linux_color::BLUE, true },
                    { /*color::GREEN,*/ attr_type::linux_color::GREEN, true },
                    { /*color::CYAN,*/ attr_type::linux_color::CYAN, true },
                    { /*color::RED,*/ attr_type::linux_color::RED, true },
                    { /*color::MAGENTA,*/ attr_type::linux_color::MAGENTA, true },
                    { /*color::YELLOW,*/ attr_type::linux_color::YELLOW, true },
                    { /*color::WHITE,*/ attr_type::linux_color::WHITE, true }
                };
                std::size_t idx = static_cast<std::size_t>(c);

                bright_ = cinfo[idx].bright_;
                color_ = static_cast<int>(cinfo[idx].lcolor_);
                color_ += foreground ? 30 : 40;
            }

            ///
            /// \brief prepare the color attribute as expected by Linux
            ///
            std::string value() const {
                constexpr char esc = (char)27;
                std::stringstream ss;
                ss << esc << "[";
                if (bright_) {
                    ss << "1;";
                }

                ss << color_ << "m";
                return ss.str();
            }
        };

        attr_type attr_;    // foreground attribute
        attr_type battr_;   // background attribute

    public:
        ///
        /// \brief set console foreground color
        /// \param c Color to be set
        ///
        void set_color(color c) noexcept {
            attr_.set_color(c);
        }

        ///
        /// \brief set console background color
        /// \param c Color to be set
        ///
        void set_background(color c) noexcept {
            battr_.set_color(c, false);
        }

        void reset() {
            attr_.reset();
            battr_.reset();
        }

        ///
        /// \brief apply
        ///
        void apply() const {
            std::cout << attr_.value();
            std::cout << battr_.value();
        }
    };

} // namespace console

#endif // LINUX_CONSOLE_H__

