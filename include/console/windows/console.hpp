
/*!
 * \brief C++ console framework
 * \file  console.hpp
 */

#if !defined(WINDOWS_CONSOLE_H__)
#define WINDOWS_CONSOLE_H__

//
#include "console/types.hpp"
#include "utils/timer.hpp"
#include "buffer.hpp"
#include "attribute.hpp"
#include <algorithm>
#include <map>
#include <iostream>

namespace xtd {

    /*!
     * \brief events structure
     */
    struct events {
    public:
        // event types
        enum class kind {
            none,
            key = KEY_EVENT,
            mouse = MOUSE_EVENT,
            resize = WINDOW_BUFFER_SIZE_EVENT,
            menu = MENU_EVENT,
            focus = FOCUS_EVENT
        };

        // mouse buttons
        enum class button {
            none,
            left = FROM_LEFT_1ST_BUTTON_PRESSED,
            right = RIGHTMOST_BUTTON_PRESSED,
            middle = FROM_LEFT_2ND_BUTTON_PRESSED
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

    private:
        handle_type h_;

    public:
        events(handle_type h) :
            h_(h)
        {
        }
        ~events() {}

        //
        operator handle_type() const noexcept {
            return h_;
        }

        /*!
         * \brief Read the next available console event
         * \return
         */
        event_type read() {
            event_type e;
            INPUT_RECORD ir;
            DWORD read_events = 0;

            if (!GetNumberOfConsoleInputEvents(h_, &read_events) || (0 == read_events)) {
                return e;
            }

            // Read the console events into that buffer, and save how
            // many events have been read into read_events.
            //            ReadConsoleInput(cin, eventBuffer, events_count, &read_events);
            if (!::ReadConsoleInput(h_, &ir, 1, &read_events)) {
                return e;
            }

            //wchar_t tmp[64];
            //wsprintf(tmp, L"read_events : %d", read_events);
            //debug_info(tmp);

            if (0 == read_events) {
                return e;
            }

            switch (ir.EventType) {
            case KEY_EVENT:
                e.type = kind::key;
                e.key = ir.Event.KeyEvent.uChar.AsciiChar;
                break;

            case MOUSE_EVENT:
                if (0 == ir.Event.MouseEvent.dwEventFlags) {
                    // zero indicates a mouse button being pressed or released
                    e.type = kind::mouse;
                    e.mouse.x = ir.Event.MouseEvent.dwMousePosition.X;
                    e.mouse.y = ir.Event.MouseEvent.dwMousePosition.Y;
                    e.mouse.st = (0 != ir.Event.MouseEvent.dwButtonState) ? state::pressed : state::released;

                    switch (ir.Event.MouseEvent.dwButtonState) {
                    case FROM_LEFT_1ST_BUTTON_PRESSED:
                        e.mouse.btn = button::left;
                        break;

                    case FROM_LEFT_2ND_BUTTON_PRESSED:
                        e.mouse.btn = button::middle;
                        break;

                    case RIGHTMOST_BUTTON_PRESSED:
                        e.mouse.btn = button::right;
                        break;
                    }
                }
                break;
            }

            ::FlushConsoleInputBuffer(h_);
            return e;
        }

    private:
        /*!
         * \brief Retrieves the number of unread input records in the
         *        console's input buffer.
         * \return
         */
        std::size_t count() const noexcept {
            DWORD count = 0;
            if (!::GetNumberOfConsoleInputEvents(h_, &count)) {
                count = 0;
            }

            return count;
        }

    };


    ///
    /// \brief screen_base struct
    ///
    template <typename T>
    struct screen_base {
        using color_type = xtd::color;

        /*!
         * \brief The visibility enum
         */
        enum class visibility {
            invisible = 0,
            normal = 1,
//            high = 2
        };

        /*!
         * \brief The alignment enum
         */
        enum class alignment {
            left,
            right
        };


    private:
        using underlying_type = T;

        handle_type handle() const {
            underlying_type const& derived = static_cast<underlying_type const&>(*this);
            return derived.output();
        }

    protected:
        struct color_pair {
            color color_;
            color background_;
        };

        std::map<int, color_pair> colors_;

    public:
        screen_base() {
        }

        virtual ~screen_base() {
        }

        /*!
         * \brief set_font
         * \param finfo
         */
        void set_font(font_type const& finfo) {
            CONSOLE_FONT_INFOEX cfi;
            cfi.cbSize = sizeof(cfi);
            cfi.nFont = 0;
            cfi.dwFontSize.X = finfo.width_;
            cfi.dwFontSize.Y = finfo.height_;
            cfi.FontFamily = FF_DONTCARE;
            cfi.FontWeight = FW_NORMAL;
            wcscpy_s(cfi.FaceName, finfo.name_);	//L"Consolas");

            if (!::SetCurrentConsoleFontEx(handle(), false, &cfi)) {
                debug_info(L"screen::set_font()");
                assert(false);
                // throw exception
//                return Error(L"SetCurrentConsoleFontEx");
            }

            //aici
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            ::GetConsoleScreenBufferInfo(handle(), &csbi);
        }

        /*!
         * \brief set_color
         * \param id
         * \param fcolor
         * \param bcolor
         */
        void add_attr(
            int id,
            color fcolor = color::white,
            color bcolor = color::black
            )
        {
            colors_[id] = color_pair{fcolor, bcolor};
        }

        void set_mode(
            ulong_t flags = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT
            )
        {
            //
            // set flags to allow mouse input
            if (!::SetConsoleMode(handle(), flags)) {
                debug_info(L"screen::set_mode()");
                assert(false);
            }
        }

        void check_error() {
			// throw exception
			debug_info(L"screen::set_window()");
			// throw exception
			//assert(false);
        }

        /*!
         * \brief get console window size
         * \return
         */
        rec_type window() const noexcept {
            CONSOLE_SCREEN_BUFFER_INFO info;
            if (!::GetConsoleScreenBufferInfo(handle(), &info)) {
                debug_info(L"screen::window()");
                assert(false);
                // throw exception
            }

            return info.srWindow;
        }

        /*!
         * \brief Set the console window size
         * \param size
         *
         * \note
         * set_window() must be called before set_window()
         */
        void set_window(coord_type const& size) {
            rec_type rec = { 0, 0, size.X - 1, size.Y - 1 };

            if (!::SetConsoleWindowInfo(handle(), TRUE, &rec)) {
                // throw exception
                debug_info(L"screen::set_window()");
                // throw exception
                assert(false);
            }
        }

        /*!
         * \brief get screen buffer size
         * \return
         */
        coord_type size() const noexcept {
            CONSOLE_SCREEN_BUFFER_INFO info;
            if (!::GetConsoleScreenBufferInfo(handle(), &info)) {
                debug_info(L"screen::size()");
                assert(false);
                // throw exception
            }

            return info.dwSize;
        }

        /*!
         * \brief resize screen buffer
         * \param size The new screen buffer size
         *
         * \note
         * set_size() must be called after set_window(), otherwise it returns
         * ERROR_INVALID_PARAMETER (87) error.
         */
        void set_size(coord_type const& size) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;  

            ::GetConsoleScreenBufferInfo(handle(), &csbi);
            coord_type wsize = make_coord(
                csbi.srWindow.Right - csbi.srWindow.Left + 1,
                csbi.srWindow.Bottom - csbi.srWindow.Top + 1
            );

            if ((wsize.X > size.X) || (wsize.Y > size.Y)) {
                // window size needs to be adjusted before the buffer size can be reduced
                rec_type rec;

                rec.Left = 0;
                rec.Top = 0;
                rec.Right = std::min(wsize.X, size.X) - 1;
                rec.Bottom = std::min(wsize.Y, size.Y) - 1;
                if (!::SetConsoleWindowInfo(handle(), TRUE, &rec)) {
                    check_error();
                }
            }

            if (!::SetConsoleScreenBufferSize(handle(), size)) {
                // throw exception
                debug_info(L"screen::set_size()");
                // throw exception
                assert(false);
            }
        }

        /*!
         * \brief show/hide cursor
         * \param visible
         */
        visibility cursor_state() const {
            CONSOLE_CURSOR_INFO cci;

            assert(nullptr != handle());
            if (!::GetConsoleCursorInfo(handle(), &cci)) {
                debug_info(L"screen::show_cursor()");
                // throw exception
                assert(false);
            }

            return cci.bVisible ? visibility::normal : visibility::invisible;
        }

        /*!
         * \brief show/hide cursor
         * \param visible
         */
//        void show_cursor(bool visible = true) const {
        void show_cursor(visibility state = visibility::normal) const {
            CONSOLE_CURSOR_INFO cci;

            assert(nullptr != handle());
            if (!::GetConsoleCursorInfo(handle(), &cci)) {
                debug_info(L"screen::show_cursor()");
                // throw exception
                assert(false);
            }

//            cci.bVisible = visible;
            cci.bVisible = (state == visibility::normal) ? 1 : 0;
            ::SetConsoleCursorInfo(handle(), &cci);
        }

        /*!
         * \brief read current cursor position
         * \return
         */
        coord_type cursor() const {
            CONSOLE_SCREEN_BUFFER_INFO csbi;

            assert(nullptr != handle());
            if (!::GetConsoleScreenBufferInfo(handle(), &csbi)) {
                debug_info(L"screen::cursor()");
                // throw exception
                assert(false);
            }

            return csbi.dwCursorPosition;
        }

        /*!
         * \brief move screen cursor to specified position
         * \param pos
         */
        void move_cursor(coord_type pos) {
            assert(nullptr != handle());
            if (!::SetConsoleCursorPosition(handle(), pos)) {
                debug_info(L"screen::move_cursor()");
                // throw exception
                assert(false);
                return;
            }
        }
    };

    ///
    /// \brief non buffered screen console
    ///
    template <typename T>
    struct screen_default :
        public screen_base<screen_default<T>>
    {
        using char_type = T;
        using color_type = xtd::color;

    protected:
        events cin_;
        handle_type cout_;
        coord_type size_;
        CONSOLE_SCREEN_BUFFER_INFO csbi_;

//    public:
        screen_default(coord_type size) :
            cin_(::GetStdHandle(STD_INPUT_HANDLE)),
            cout_(::GetStdHandle(STD_OUTPUT_HANDLE)),
            size_(size)
        {
            ::GetConsoleScreenBufferInfo(cout_, &csbi_);
//            std::wcout << L"size: " << csbi_.dwSize.X << L", " << csbi_.dwSize.Y << L"\n";
        }

        ~screen_default() noexcept {
//            std::wcout << L"restore size: " << csbi_.dwSize.X << L", " << csbi_.dwSize.Y << L"\n";
            ::SetConsoleScreenBufferSize(cout_, csbi_.dwSize);
            ::SetConsoleWindowInfo(cout_, TRUE, &csbi_.srWindow);
        }

    public:
        handle_type input() const noexcept {
            return cin_;
        }

        handle_type output() const noexcept {
            return cout_;
        }

        coord_type size() const noexcept {
            return size_;
        }

        ///
        /// \brief set_attr
        /// \param color
        /// \param bcolor
        ///
//        void set_attr(xtd::console::color color, xtd::console::color bcolor) {
        void set_attr(
            color_type color = color_type::white,
            color_type bcolor = color_type::black
            )
        {
            // TODO:
            console_attr attr;
            attr.set_color(color);
            attr.set_background(bcolor);
            attr.apply();
        }

        std::size_t print(char_type c) {
            DWORD written;
            ::WriteConsole(cout_, &c, 1, &written, nullptr);
            return written;
        }

        std::size_t print(const char_type* s) {
            DWORD written = 0;
            DWORD count = _tcslen(s);
            ::WriteConsole(cout_, s, count, &written, nullptr);
            return written;
        }

        void flush() {
        }
    };


    ///
    /// \brief buffered screen console
    ///
    template <typename T>
    struct screen_buffered :
        public screen_base<screen_buffered<T>>
    {
        using char_type = T;
    protected:
        using base_type = screen_base<screen_buffered<char_type>>;
        using buffer_type = buffer<char_type>;

        events cin_;
        handle_type orig_cout_;
        handle_type cout_;
        buffer_type buff_;
        streambuf<char_type> stmbuff_;
        std::basic_streambuf<char_type>* orig_stmbuff_;

//    public:
        screen_buffered(coord_type size) :
            cin_(::GetStdHandle(STD_INPUT_HANDLE)),
            orig_cout_(::GetStdHandle(STD_OUTPUT_HANDLE)),
            buff_(size),
            stmbuff_(buff_)
        {
            cout_ = ::CreateConsoleScreenBuffer(
                GENERIC_READ | GENERIC_WRITE,		// read/write access
                FILE_SHARE_READ | FILE_SHARE_WRITE, 	// shared
                NULL,                    			// default security attributes
                CONSOLE_TEXTMODE_BUFFER, 			// must be TEXTMODE
                NULL
                );
            if (INVALID_HANDLE_VALUE == cout_) {
                debug_info(L"screen::screen()");
                // throw exception
                assert(false);
            }

            buff_.clear();
#if defined(UNICODE)
            orig_stmbuff_ = std::wcout.rdbuf(&stmbuff_);
#else
            orig_stmbuff_ = std::cout.rdbuf(&stmbuff_);
#endif

            if (!::SetConsoleActiveScreenBuffer(cout_)) {
                OutputDebugString(L"set_screen() failed!");
                assert(false);
            }

//            if (!::SetStdHandle(STD_OUTPUT_HANDLE, cout_)) {
//                assert(false);
//            }
        }

        ~screen_buffered() noexcept {
            // restore the original screen buffer
            SetConsoleActiveScreenBuffer(orig_cout_);
//			SetStdHandle(STD_OUTPUT_HANDLE, cout_);
//            SetStdHandle(STD_OUTPUT_HANDLE, orig_cout_);
#if defined(UNICODE)
            std::wcout.rdbuf(orig_stmbuff_);
#else
            std::cout.rdbuf(orig_stmbuff_);
#endif
        }

    public:
        handle_type input() const noexcept {
            return cin_;
        }

        handle_type output() const noexcept {
            return cout_;
        }

        coord_type size() const noexcept {
            return buff_.size();
        }

        ///
        /// \brief set_attr
        /// \param color
        /// \param bcolor
        ///
        void set_attr(int id) {
            auto it = base_type::colors_.find(id);
            if (base_type::colors_.end() != it) {
                buff_.set_color((*it).second.color_);
                buff_.set_background((*it).second.background_);
            }
        }

        /*!
         * \brief read current cursor position
         * \return
         */
        coord_type cursor() const {
            return buff_.cursor();
        }

        /*!
         * \brief move screen cursor to specified position
         * \param pos
         */
        void move_cursor(coord_type pos) {
//            base_type::move_cursor(pos);
            buff_.move_cursor(pos);
        }

        ///
        /// \brief print
        /// \param s
        /// \return
        ///
        std::size_t print(char_type c,  attr_type attr = 0) {
            return buff_.print(c, attr);
        }

        ///
        /// \brief print
        /// \param s
        /// \return
        ///
        std::size_t print(const char_type* s,  attr_type attr = 0) {
            return buff_.print(s, attr);
        }

        void clear_area(coord_type top_left, coord_type bottom_right) {
            buff_.clear_area(top_left, bottom_right);
        }

        void clear() {
            buff_.clear();
        }

        void refresh() {
            xtd::timer t;
            wchar_t tmp[64];

            coord_type orig = { 0, 0 };
//            rec_type area = { 0, 0, (short_type)(buff_.size().X - 1), (short_type)(buff_.size().Y - 1) };
            rec_type rc = buff_.refresh_frame();
            rec_type area = {
                rc.Left,
                rc.Top,
                (short_type)(rc.Right),
                (short_type)(rc.Bottom)
            };
            t.start();
            bool r = ::WriteConsoleOutputA(
                cout_,
                buff_.cells(),
                buff_.size(),
                orig,
                &area
                );
            if (!r) {
                // throw exception
                debug_info(L"screen::write()");
                // throw exception
                assert(false);
            }

            using timer_units = std::chrono::milliseconds;
            auto d1 = t.stop<timer_units>();
            //wsprintf(tmp, L"refress() : %d ms", d1);
            //debug_info(tmp);

            buff_.reset_refresh_frame();
            base_type::move_cursor(buff_.cursor());
        }
    };


    ///
    /// \brief console struct
    ///
    template <typename T>
    struct console :
        public T
    {
    private:
        using base_type = T;

    public:
        console(coord_type size) :
            base_type(size)
        {
        }

        ~console() noexcept {
        }

        /*!
         * \brief get events
         * \return
         */
        events& get_events() noexcept {
            return base_type::cin_;
        }

        /*!
         * \brief set console title
         * \param title
         */
        void set_title(wchar_t const* title) const noexcept {
            ::SetConsoleTitle(title);
        }
    };


    ///
    /// \brief operator <<
    /// \param out
    /// \param c
    /// \return
    ///
    std::ostream& operator<<(std::ostream& out, console_attr const& attr) {
        attr.apply();
        return out;
    }

} // namespace xtd

#endif // WINDOWS_CONSOLE_H__
