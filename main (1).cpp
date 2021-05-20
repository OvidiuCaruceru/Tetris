#include "engine.h"
#include "timer.hpp"
#include <chrono>

int main() {
    using namespace xtd;
    const short_type width = 80;
    const short_type height = 30;
    coord_type size = make_coord(width, height);
    console_type con(size);
    events& ev = con.get_events();
    timer tmr;

    #if defined(OS_WIN)
    con.set_size(size);
    con.set_window(size);
    #endif
    con.add_attr(0, color::white, color::black);
    con.add_attr(1, color::white, color::darkyellow);
    con.add_attr(2, color::white, color::darkred);
    con.add_attr(3, color::white, color::magenta);
    con.add_attr(4, color::white, color::red);
    con.add_attr(5, color::white, color::blue);
    con.add_attr(6, color::white, color::darkgreen);
    con.add_attr(7, color::white, color::darkcyan);
    con.add_attr(8, color::white, color::darkgray);
    con.show_cursor(console_type::visibility::invisible);

    engine eng(20, 12, 4, 4);
    eng.draw(con);
    tmr.start();

    while (true) {
        eng.handle_event(ev.read());
        if (tmr.elapsed() > eng.speed()) {
            tmr.stop();
            events::event_type e;
            e.type = events::kind::key;
            // simulates downward movement
            e.key = 's';
            eng.handle_event(e);

            if (eng.game_over()) {
                break;
            }
            eng.draw(con);
            tmr.start();
        }

        xtd::timer::sleep(std::chrono::milliseconds(50));
    }

    con.set_attr(0);
    con.move_cursor(make_coord(eng.board_width(), eng.board_height() / 2 ));
    con.print(L"Game over!");
    con.refresh();

    xtd::timer::sleep(std::chrono::milliseconds(5000));

    return 0;
}

