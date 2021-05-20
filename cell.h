
/*!
 * \file cell.hpp
 * \brief class used as building block for tetromino blocks and board
 */

#if !defined (_CELL_H__)
#define _CELL_H__

///
/// \brief state used to determine how a cell will behave
///        and interact in tetromino blocks and board
/// I, O, T, L, J, S, Z used to change the color of the block
///        based on its shape
///
enum class state {
    empty,
    wall,
    I,
    O,
    T,
    L,
    J,
    S,
    Z
};

///
/// \brief class used as building block for tetromino blocks and board
///
struct cell {
    state state_;

    cell() :
        state_(state::empty)
    {
    }
};

#endif // CELL_H__