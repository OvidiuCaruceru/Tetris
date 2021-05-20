
/*!
 * \file board.hpp
 * \brief class which implements the logic of the game
 */

#if !defined (_ENGINE_H__)
#define _ENGINE_H__

#include "blocks.hpp"
#include "../include/utils/random.hpp"

#include <string>
#include <stdio.h>

///
/// \brief class which implements the logic of the game
///
struct engine {
	using string_type = std::string;

///
/// \brief finish_ true leads to game over
///			pattern_ is a string applied to block to set its shape and color
///			rng_ uses a random number generator from class random
/// 
private:
	bool finish_;
	std::vector<string_type> pattern_;
	board board_;
	block block_;
	int score_;
	xtd::random<int> rng_;
	int speed_;

public:
	engine() = delete;
	~engine() = default;

	engine(short_type rows_board, short_type columns_board, short_type rows_block, short_type columns_block) :
		finish_(false),
		// the patterns, stored as strings, which will be applyed to block to give its shape
		pattern_(
			{
				"....IIII........",
				".....OO..OO.....",
				"......T..TTT....",
				".....L...L...LL.",
				"......J...J..JJ.",
				".....S...SS...S.",
				"......Z..ZZ..Z.."
			}
			),
		board_(rows_board, columns_board),
		block_(
			rows_block,
			columns_block,
			make_coord(
				board_.width() / 2 - columns_block / 2,
				0
				)
			),
		score_(0),
		// generate a random number to choose one of the patterns
		rng_(0, 6),
		speed_(500)
	{	// constructor initiates a block with random pattern
		block_.init(pattern_[rng_.seed()]);
	}

	size_type board_width() const noexcept {
		return board_.width();
	}

	size_type board_height() const noexcept {
		return board_.height();
	}

	int score() const noexcept {
		return score_;
	}

	int speed() const noexcept {
		return speed_;
	}

	///
	/// \brief	prints the board, block and score in the console
	/// 
	void draw(console_type& con) {
		board_.print(con);
		block_.print(con, board_.origin());
		draw_score(con);
		con.refresh();
	}

	///
	/// \brief	moves the block based on keyboard inputs by creating a temporary
	///			block and checking for overlap between tmp block's cells and board cells
	///
	void handle_event(events::event_type ev) {
		if (events::kind::key == ev.type) {
			block tmp(block_);
			bool changed = true;
			bool down = false;

			switch (ev.key) {
			case 'Q':
			case 'q':
				changed = false;
				finish_ = true;
				break;
			case 'W':
			case 'w':
				tmp.rotate_right();
				break;
			case 'A':
			case 'a':
				tmp.move_left();
				break;
			case 'D':
			case 'd':
				tmp.move_right();
				break;
			case 'S':
			case 's':
				down = true;
				tmp.move_down();
				break;
			// moves block to the lowest available position underneath it
			case ' ':
				down = true;
				while (!overlap(tmp)) {
					tmp.move_down();
				}
				// moves block up before applying because the tmp block will always overlap not empty cells in its final position
				tmp.move_up();
				break;
			default:
				changed = false;
				break;
			}

			if (changed) {
				if (!overlap(tmp)) {
					block_ = std::move(tmp);
				}
				// after the block is applied to the board, board is tested for full rows and a new block is created
				else if (down) {
					apply_block();
					test_for_full_rows();
					create_block();
				}
			}
		}
	}

	///
	/// \brief	checks block's cells one by one and board cells underneath block for overlap
	///			returns true if overlap
	///
	bool overlap(block const& b) const {
		for (auto x = 0u; x < b.width(); ++x) {
			for (auto y = 0u; y < b.height(); ++y) {
				if (b[y][x].state_ != state::empty) {
					coord_type pos = b.position();
					if (board_[pos.Y + y][pos.X + x].state_ != state::empty) {
						return true;
					}
				}
			}
		}

		return false;
	}

	///
	/// \brief	changes the board's cells state based on the block's cells state
	///
	void apply_block() {
		coord_type pos = block_.position();
		for (auto x = 0u; x < block_.width(); ++x) {
			for (auto y = 0u; y < block_.height(); ++y) {
				if (block_[y][x].state_ != state::empty) {
					board_[pos.Y + y][pos.X + x].state_ = block_[y][x].state_;
				}
			}
		}
	}

	///
	/// \brief	tests if rows are full, deletes full rows and updates score and speed
	///
	void test_for_full_rows() {
		bool full_row = false;
		int rows = 0;

		for (int y = board_.height() - 2; y > 0; --y) {
			for (int x = board_.width() - 1; x > 0; --x) {
				if (board_[y][x].state_ != state::empty) {
					full_row = true;
				}
				else {
					full_row = false;
				}
				if (!full_row)
					break;
			}

			if (full_row) {
				++rows; 
				for (int i = y; i > 0; --i) {
					for (int j = board_.width() - 1; j > 0; --j) {
						board_[i][j] = board_[i - 1][j];
					}
				}

				++y; // goes to the next rows

				if (rows > 0 && rows < 4) {
					score_ += 10;
					if (speed_ > 150) {
						speed_ -= 5;
					}
					rows = 0;
				}
				else if (rows >= 4) {
					score_ += 100;
					if (speed_ > 150) {
						speed_ -= 10;
					}
					rows = 0;
				}
			}
		}
	}

	///
	/// \brief	ends the game by setting finish_ to true if board's upper cells are full
	///
	bool game_over() {
		for (auto x = 0u; x < 2; ++x) {
			for (auto y = (board_.width() / 2) - (block_.width() / 2); y < (board_.width() / 2) + (block_.width() / 2); ++y) {
				if (board_[x][y].state_ != state::empty) {
					finish_ = true;
				}
			}
		}
		return finish_;
	}
	
	///
	/// \brief	prints score in the console
	///
	void draw_score(console_type& con) {
		con.set_attr(2);
		con.move_cursor(make_coord(2, board_.height() + 1));
		con.print(L"Score: ");
		// transforms integers to text for print
		wchar_t tmp[32];
		_snwprintf_s(tmp, sizeof(tmp) / sizeof(tmp[0]), L"%d", score_);
		con.print(tmp);
	}

private:
	///
	/// \brief	creates a new block after block apply to board
	///
	void create_block() {
		block_.init(pattern_[rng_.seed()]);
		block_.set_position(make_coord(board_.width() / 2 - block_.width() / 2, 0));
	}
}; // struct engine

#endif // ENGINE_H__