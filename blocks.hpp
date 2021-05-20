
/*!
 * \file blocks.hpp
 * \brief class to crate the tetromino blocks
 */

#if !defined (_BLOCKS_H_)
#define _BLOCKS_H_

#include "board.hpp"

using namespace xtd;
#if defined(OS_LINUX)
using console_type = console;
#elif defined(OS_WIN)
using console_type = xtd::console<xtd::screen_buffered<wchar_t>>;
#endif

///
/// \brief class used to create tetromino blocks
///
struct block {
    using matrix_type = matrix<cell>;
    using line_type = matrix_type::line_type;
    using size_type = matrix_type::size_type;
    using string_type = std::string;
   

///
/// \brief cells_ used to make tetromino block using cell
///         pos_ used to adjust block's position on board/in console
///
private:
    matrix_type cells_;
    coord_type pos_;

public:
    block() = delete;
    block(size_type rows, size_type columns, coord_type pos) :
        cells_(rows, columns),
        pos_(pos)
    {

    }

    ~block() = default;

    block(block const& rhs) = default;
    block& operator=(block const&) = default;
    block(block&&) = default;
    block& operator= (block&&) = default;

    line_type& operator[](size_type i) {
        return cells_[i];
    }

    line_type const& operator[](size_type i) const {
        return cells_[i];
    }

    ///
    /// \brief get block width
    ///
    size_type width() const noexcept {
        return cells_.columns();
    }

    ///
    /// \brief get block height
    ///
    size_type height() const noexcept {
        return cells_.rows();
    }

    ///
    /// \brief get block position
    ///
    coord_type position() const noexcept {
        return pos_;
    }   

    ///
    /// \brief set block position
    ///
    void set_position(coord_type pos) {
        pos_ = pos;
    }

    ///
    /// \brief change the state of each cell based on it's shape
    ///         in order to set different colors for each block
    ///
    void init(string_type string) {
        for (auto i = 0u; i < string.length(); ++i) {
            if (string[i] == 'I') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::I;
            }
            else if (string[i] == 'O') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::O;
            }
            else if (string[i] == 'T') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::T;
            }
            else if (string[i] == 'L') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::L;
            }
            else if (string[i] == 'J') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::J;
            }
            else if (string[i] == 'S') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::S;
            }
            else if (string[i] == 'Z') {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::Z;
            }
            else {
                cells_[i / cells_.rows()][i % cells_.columns()].state_ = state::empty;
            }
        }
    }
    
    ///
    /// \brief block position change
    ///
    void move_right() {
        ++pos_.X;
    }

    void move_left() {
        --pos_.X;
    }

    void move_down() {
        ++pos_.Y;
    }

    void move_up() {
        --pos_.Y;
    }

    //void rotate_block_left(block& b) {
    //    for (unsigned long x = 0; x < b.height_block() / 2; ++x) {
    //        for (unsigned long y = x; y < b.height_block() - x - 1; y++) {
    //            auto tmp = b[x][y];
    //            b[x][y] = b[y][b.height_block() - 1 - x];
    //            b[y][b.height_block() - 1 - x] = b[b.height_block() - 1 - x][b.height_block() - 1 - y];
    //            b[b.height_block() - 1 - x][b.height_block() - 1 - y] = b[b.height_block() - 1 - y][x];
    //            b[b.height_block() - 1 - y][x] = tmp;
    //        }
    //    }
    //}


    ///
    /// \brief goes through each cell and changes its position
    ///         within the block, thus changing the pattern of the block,
    ///         simulating a clockwise rotation
    ///
    void rotate_right() {
        for (auto x = 0u; x < height() / 2; ++x) {
            for (auto y = x; y < height() - x - 1; ++y) {
                auto tmp = cells_[x][y];
                cells_[x][y] = cells_[height() - 1 - y][x];
                cells_[height() - 1 - y][x] = cells_[height() - 1 - x][height() - 1 - y];
                cells_[height() - 1 - x][height() - 1 - y] = cells_[y][height() - 1 - x];
                cells_[y][height() - 1 - x] = tmp;
            }
        }
    }

    ///
    /// \brief used to print the block in the console and assign
    ///         a different color based on its shape/cell state
    ///
    void print(console_type& con, coord_type orig) {
        for (auto x = 0u; x < cells_.columns(); ++x) {
            for (auto y = 0u; y < cells_.rows(); ++y) {
                cell const& c = cells_[y][x];
                bool color = true;
                switch (c.state_) {
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
                    color = false;
                    ;

                }

                ///
                /// \brief this moves the console's coursor at a certail position
                ///         in order to print the block there
                ///
                if (color) {
                    con.move_cursor(make_coord(orig.X + (pos_.X + x) * 2, orig.Y + (pos_.Y - 1) + y));
                    con.print(' ');
                    con.refresh();
                    con.print(' ');
                    con.refresh();
                }
            }
        }
    }
}; // struct block

#endif //BLOCKS_H_

