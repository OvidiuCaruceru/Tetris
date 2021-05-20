
/*!
 * \brief C++ console framework (Linux)
 * \file  console.hpp
 */

#if !defined(LINUX_CONSOLE_H__)
#define LINUX_CONSOLE_H__

#include "console/types.hpp"
#include <ncurses.h>
#include <locale.h>
#include <utility>
#include <map>
#include <string>
#include <cassert>

namespace xtd {

    ///
    /// \brief generic color code(s)
    ///
    enum class color {
        black = COLOR_BLACK,
        blue = COLOR_BLUE,
        green = COLOR_GREEN,
        cyan = COLOR_CYAN,
        red = COLOR_RED,
        magenta = COLOR_MAGENTA,
        yellow = COLOR_YELLOW,
        white = COLOR_WHITE,
//        DARKBLUE,
//        DARKGREEN,
//        DARKCYAN,
//        DARKRED,
//        DARKMAGENTA,
//        DARKYELLOW,
//        DARKGRAY,
//        GRAY,
    };

    /*!
     * \brief events structure
     */
    struct events {

        // event types
        enum class kind {
            none,
            key,
            mouse,
            resize,
        };

        // mouse buttons
        enum class button {
            none,
            left,
            right,
            middle,
        };

        enum class state {
            none,
            pressed,
            released
        };

        struct event {
            kind type;
            union {
                int key;
                struct {
                    int x;
                    int y;
                    button btn;
                    state st;
                } mouse;
            };

            event() :
                type(kind::none)
            {
            }
        };
        using event_type = struct event;

    public:
        /*!
         * \brief Read the next available console event
         * \return
         *
         * \note
         * The function does not return until at least one input event has been read.
         */
        event_type read() {
            event_type ev;
            int c;

            ev.type = kind::none;
            c = getch();
            if (ERR == c) {
                // no event available
                return ev;
            }

            if (KEY_MOUSE == c) {
                MEVENT me;
                if (OK == ::getmouse(&me)) {
                    ev.type = kind::mouse;
                    ev.mouse.x = me.x;
                    ev.mouse.y = me.y;

                    switch (me.bstate) {
                    case BUTTON1_PRESSED:
                        ev.mouse.btn = button::left;
                        ev.mouse.st = state::pressed;
                        break;
                    case BUTTON1_RELEASED:
                        ev.mouse.btn = button::left;
                        ev.mouse.st = state::released;
                        break;
                    case BUTTON2_PRESSED:
                        ev.mouse.btn = button::middle;
                        ev.mouse.st = state::pressed;
                        break;
                    case BUTTON2_RELEASED:
                        ev.mouse.btn = button::middle;
                        ev.mouse.st = state::released;
                        break;
                    case BUTTON3_PRESSED:
                        ev.mouse.btn = button::right;
                        ev.mouse.st = state::pressed;
                        break;
                    case BUTTON3_RELEASED:
                        ev.mouse.btn = button::right;
                        ev.mouse.st = state::released;
                        break;
                    }
                }
            } else {
                ev.type = kind::key;
                ev.key = c;
            }

            return ev;
        }
    };


    /*
     * \brief screen_base struct
     */
    struct console{
        using char_type = traits::char_type;
//        using color_type = short_type;
        using color_type = color;
        using pair_type = std::pair<color_type, color_type>;

        /*!
         * \brief The visibility enum
         */
        enum class visibility {
            invisible = 0,
            normal = 1,
            high = 2
        };

        /*!
         * \brief The alignment enum
         */
        enum class alignment {
            left,
            right
        };

    private:
        using colors_type = std::map<int, pair_type>;

        rec_type r_;
        color_type color_;
        color_type background_;
        colors_type colors_;
        int key_;
        events ev_;

    public:
        console(coord_type) :
            color_(color::white),
            background_(color::black),
            key_(-1)
        {
            /* make sure UTF8 */
            ::setlocale(LC_ALL, "");
//            ::setlocale(LC_ALL, "en_US.UTF-8");

            /* initialize curses */
            ::initscr();

            // set non-blocking read
//            ::timeout(0);
            set_timeout(0);

            raw();
            if ( ::has_colors() ) {
                start_color();
            }

            keypad(stdscr, TRUE);
            cbreak();
            noecho();
//            nonl();
//            intrflush(stdscr, FALSE);
            /* Enable keypad mode */
//            keypad(stdscr, TRUE);

            // initialize mounse
            mousemask(ALL_MOUSE_EVENTS /*| REPORT_MOUSE_POSITION*/, NULL);
            assert(has_mouse());

            // to get snappier button press/release events
            mouseinterval(0);

            // make the terminal report mouse movement events
            // https://www.xfree86.org/4.8.0/ctlseqs.html
            // (xterm control sequence)
//            printf("\033[?1003h\n");

            clear();

            r_ = make_rec(0, 0, COLS - 1, LINES - 1);
        }

        ~console() {
            // disable mouse movement events, as l = low
//            printf("\033[?1003l\n");
            attrset(A_NORMAL);
            ::endwin();
            ::fflush(stdout);
        }

        /*!
         * \brief timeout
         * \param tm
         *      > 0 timeout
         *      = 0 non-blocking
         *      < 0 blocking
         */
        void set_timeout(int tm) {
            ::timeout(tm);
        }

        events& get_events() noexcept {
            return ev_;
        }

        /*!
         * \brief set_font
         * \param finfo
         * \note Not yet supported!
         */
//        void set_font(font_type const& /*finfo*/) {
//            assert(false);
//        }

        /*!
         * \brief get console window size
         * \return
         */
        rec_type window() const noexcept {
            return r_;
        }

        /*!
         * \brief Set the console window size
         * \param size
         *
         * \note
         * set_window() must be called before set_window()
         */
        void set_window(rec_type const& /*rec*/) {
        }

        /*!
         * \brief get screen buffer size
         * \return
         * \note Not yet supported!
         */
        coord_type size() const noexcept {
            assert(false);
            return coord_type{};
        }

        /*!
         * \brief resize screen buffer
         * \param size The new screen buffer size
         * \note Not yet supported!
         */
        void set_size(coord_type const& /*size*/) {
        }

        void add_attr(
            int key,
            color_type color /*= COLOR_WHITE*/,
            color_type bcolor = color::black
            )
        {
            colors_[key] = std::pair<color_type, color_type>(color, bcolor);
            ::init_pair(
                key,
                static_cast<int>(color),
                static_cast<int>(bcolor)
                );
        }

        /*
         * \brief
         * \param key
         */
        void set_attr(
            int key
            )
        {
            colors_type::const_iterator it = colors_.find(key);
            if (it != colors_.end()) {
                if (key_ >= 0) {
                    ::attroff(COLOR_PAIR(key_));
                }

                key_ = (*it).first;
                ::attron(COLOR_PAIR(key_));
            } else {
                assert(false);
            }
        }

        /*!
         * \brief show/hide cursor
         * \param visible
         */
        visibility cursor_state() const {
            visibility state = static_cast<visibility>(::curs_set(0));
            curs_set(static_cast<int>(state));
            return state;
        }

        /*!
         * \brief show/hide cursor
         * \param visible
         */
        void show_cursor(visibility state = visibility::normal) const {
            ::curs_set(static_cast<int>(state));
        }

        /*!
         * \brief read current cursor position
         * \return
         */
        coord_type cursor() const {
            short_type x;
            short_type y;

            getyx(stdscr, y, x);
            return make_coord(x, y);
        }

        /*
         * \brief move screen cursor to specified position
         * \param pos The new cursor position
         */
        void move_cursor(coord_type pos) {
            ::move(pos.Y, pos.X);
        }

        /*
         * \brief write a character to the terminal
         * \param c
         * \return
         * \note
         * The character is written at the cursor possion and the
         * cursor position is moved to the right with one posision.
         */
        std::size_t print(char_type c) {
            coord_type pos = cursor();
            mvaddch(pos.Y, pos.X, c);

            return 1;
        }

        /*
         * \brief write the string to the terminal
         * \param str   String to be witten to terminal
         * \param width Number of characters to be writte
         *              (if the string is smaller, it will be padded with
         *              spaces up to the required length).
         *              If this value is -1, no padding is involved.
         * \param align Text alignment, i.e. to the left or to
         *              the right.
         *
         * \return
         * \note
         * The string is written at the cursor possion and the
         * cursor position is moved to the right corresponding to
         * the number characters in the string.
         */
        std::size_t print(
            const char_type* str,
            int width = -1,
            alignment align = alignment::left
            )
        {
            coord_type pos = cursor();
            std::string tmp = str;

            if (width > 0) {
                switch (align) {
                case alignment::left:
                    tmp.insert(tmp.length(), width - tmp.size(), ' ');
                    break;

                case alignment::right:
                default:
                    tmp.insert(0, width - tmp.size(), ' ');
                    break;
                }
            }

            mvaddstr(pos.Y, pos.X, tmp.c_str());
//            mvprintw(str);
            return tmp.length();
        }

        /*
         * \brief write the internal buffer content to the terminal
         */
        void refresh() {
            ::refresh();
        }

        /*
         * \brief clear a termina area
         * \param top_left
         * \param bottom_right
         */
        void clear_area(coord_type top_left, coord_type bottom_right) {
            coord_type pt = top_left;
            auto width = (bottom_right.X - top_left.X);
            auto height = (bottom_right.Y - top_left.Y);

            for (auto j = 0; j < height; ++j) {
                move_cursor(pt);
                for (auto i = 0; i < width; ++i) {
                    print(" ");
                }

                ++pt.Y;
            }
        }

        /*!
         * \brief set console title
         * \param title
         */
        void set_title(char_type const* /*title*/) const noexcept {
//            std::cout << "\033]0;" << title << "\007";
        }
    };

} // namespace xtd

#endif // LINUX_CONSOLE_H__
