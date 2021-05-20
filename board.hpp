
/*!
 * \file board.hpp
 * \brief class to crate the board/playing area
 */

#if !defined(_BOARD_H_)
#define _BOARD_H_

#include "cell.h"
#include "matrix.hpp"
#include "console/console.hpp"
#include "console/types.hpp"
#include "console/gdi.hpp"

using namespace xtd;
#if defined(OS_LINUX)
using console_type = console;
#elif defined(OS_WIN)
using console_type = xtd::console<xtd::screen_buffered<wchar_t>>;
#endif


///
/// \brief class used to create the board/playing area
///
struct board {
    using matrix_type = matrix<cell>;
    using line_type = matrix_type::line_type;
    using size_type = matrix_type::size_type;

///
/// \brief cells_ used to make block using class cell
///         orig_ used to store board's starting position
///
private:
    matrix_type cells_;
    coord_type orig_;

public:
    board() = default;

    ///
    /// \brief param constructor sets the board's walls 
    ///
    board(size_type rows, size_type columns) :
        cells_(rows, columns),
        // coordinates for placing the board in console
        orig_(make_coord(2, 1))
    {

        // left and right walls
        for (auto i = 0u; i < rows; ++i) {
            cells_[i][0].state_ = state::wall;
            cells_[i][columns - 1].state_ = state::wall;
        }

        // bottom wall
        for (auto j = 0u; j < columns; ++j) {
            cells_[rows - 1][j].state_ = state::wall;
        }
    }

    ~board() = default;

    ///
    /// \brief returns board's origin coordinate
    ///
    coord_type origin() const noexcept {
        return orig_;
    }

    line_type& operator[](size_type i) {
        return cells_[i];
    }

    line_type const& operator[](size_type i) const {
        return cells_[i];
    }

    ///
    /// \brief returns board's number of columns
    ///
    size_type width() const noexcept {
        return cells_.columns();
    }

    ///
    /// \brief returns board's number of rows
    ///
    size_type height() const noexcept {
        return cells_.rows();
    }

    ///
    /// \brief used to print the board in the console and assign
    ///         different colors for wall and the blocks that are
    ///         applied on the board
    ///
    void print(console_type& con) {
        for (auto x = 0u; x < cells_.columns(); ++x) {
            for (auto y = 0u; y < cells_.rows(); ++y) {
                cell const& c = cells_[y][x];
                switch (c.state_) {
                case state::wall:
                    con.set_attr(8);
                    break;
                case state::I:
                    con.set_attr(1);
                    break;
                case state::O:
                    con.set_attr(2);
                    break;
                case state::T:
                    con.set_attr(3);
                    break;
                case state::L:
                    con.set_attr(4);
                    break;
                case state::J:
                    con.set_attr(5);
                    break;
                case state::S:
                    con.set_attr(6);
                    break;
                case state::Z:
                    con.set_attr(7);
                    break;
                default:
                    con.set_attr(0);
            }

                /// \brief this moves the console's coursor at a certail position
                ///         in order to print the block there
                ///
                con.move_cursor(make_coord(orig_.X + x * 2, orig_.Y + y));
                con.print(' ');
                con.print(' ');
            }
        }
    }

}; // struct board

#endif  //BOARD_H

