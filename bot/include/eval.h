#ifndef EVAL_H
#define EVAL_H

#include "node.h"

struct eval
{
	weight w;

	int evaluate(node& node);

	void column_height(bitboard& board, int result[10]);
	void bumpiness(int column_height[10], int result[2]);
	void blocked_cell_hole(bitboard& board, int column_height[10], int result[2]);
	void block_above_hole(bitboard& board, int column_height[10], int result[2]);
	void structure(bitboard& board, int column_height[10], int result[2]);
};

inline int eval::evaluate(node & node)
{
	int result = 0;

	int column_height[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	this->column_height(node.board, column_height);

	int highest_height = column_height[0];
	int well_index = 0;
	int well_depth = 0;

	for (int i = 0; i < 10; i++) {

		// highest height
		if (column_height[i] > highest_height)
			highest_height = column_height[i];

		// well - lowest column height
		if (column_height[i] <= column_height[well_index])
			if (column_height[i] < column_height[well_index]) {
				well_index = i;
			}
			else {
				if (i < 9) {
					well_index = i;
				}
			}
	}

	// height
	result += highest_height * w.top_height;
	result += std::max(highest_height - 10, 0) * w.top_height_top_half;
	result += std::max(highest_height - 15, 0) * w.top_height_top_half;

	// well depth
	uint16_t well_row_struct = 0b1111111111 & ~(0b1 << (9 - well_index));
	for (int i = 39 - column_height[well_index]; i >= 20; --i) {
		if ((node.board.row[i] & 0b1111111111) == well_row_struct) {
			++well_depth;
		}
		else {
			break;
		}
	}
	result += std::min(well_depth, 15) * w.well;
	if (well_depth > 0) {
		switch (well_index)
		{
		case 0:
			result += well_depth * w.well_1_10;
			break;
		case 9:
			result += well_depth * w.well_1_10;
			break;
		case 1:
			result += well_depth * w.well_2_9;
			break;
		case 8:
			result += well_depth * w.well_2_9;
			break;
		case 2:
			result += well_depth * w.well_3_8;
			break;
		case 7:
			result += well_depth * w.well_3_8;
			break;
		default:
			result += well_depth * w.well_center;
			break;
		}
	}

	// bumpiness
	int bumpiness[2] = { 0, 0 };
	this->bumpiness(column_height, bumpiness);
	result += bumpiness[0] * w.bumpiness + bumpiness[1] * w.bumpiness_s;

	// structure
	this->structure(node.board, column_height, node.t_struct);
	result += node.t_struct[0] * w.structure[0] + node.t_struct[1] * w.structure[1];

	// waste structure
	// DEFINITION: if the number of structures is smaller than the previous generation's, but there isn't any t spins, then structures are wasted
	if (node.lock != LOCK_TSPIN_1 && node.lock != LOCK_TSPIN_2 && node.lock != LOCK_TSPIN_3) {
		/*
		if (node.p_struct_tsd + node.p_struct_stsd + node.p_struct_tst > node.struct_tsd + node.struct_stsd + node.struct_tst) {
			//result += ((node.p_struct_tsd + node.p_struct_stsd + node.p_struct_tst) - (node.struct_tsd + node.struct_stsd + node.struct_tst)) * w.waste_structure;
			node.struct_wasted += ((node.p_struct_tsd + node.p_struct_stsd + node.p_struct_tst) - (node.struct_tsd + node.struct_stsd + node.struct_tst));
		}*/
		for (int i = 0; i < 2; ++i) {
			node.t_struct_wasted[i] += std::min(0, node.p_t_struct[i] - node.t_struct[i]);
		}
	}
	result += node.t_struct_wasted[0] * w.waste_structure[0] + node.t_struct_wasted[1] * w.waste_structure[1];

	// hole and blocked_cell
	int blocked_cell_hole[2] = { 0, 0 };
	this->blocked_cell_hole(node.board, column_height, blocked_cell_hole);
	result += blocked_cell_hole[0] * w.blocked_cell + blocked_cell_hole[1] * w.hole;

	// block above hole
	int block_above_hole[2] = { 0, 0 };
	this->block_above_hole(node.board, column_height, block_above_hole);
	result += block_above_hole[0] * w.block_above_hole + block_above_hole[1] * w.block_above_hole_s;

	// b2b and ren
	result += node.max_b2b_chain * w.b2b_max_chain + node.max_ren_chain * w.b2b_max_chain;
	result += node.b2b * w.b2b_chain + node.ren * w.ren_chain;

	// line sent
	result += node.line_sent * w.line_sent;

	// perfect clear
	result += node.pc * w.perfect_clear;

	// t spin
	result += node.tspin_1 * w.t_spin_1;
	result += node.tspin_2 * w.t_spin_2;
	result += node.tspin_3 * w.t_spin_3;

	// tetris
	result += node.tetris * w.tetris;

	// burn line - skimming
	result += node.burn_1 * w.burn_1;
	result += node.burn_2 * w.burn_2;
	result += node.burn_3 * w.burn_3;

	// waste time
	// DEFINITION: Time is wasted if softdrop a non-t piece, soft drop t piece without t spin, or burn lines
	int time_waste = 0;
	if ((node.path[node.path.size - 1] & 0b10) == 0b10) {
		// if just soft drop, but wasn't a t spin then wasted time
		if (node.lock != LOCK_TSPIN_1 && node.lock != LOCK_TSPIN_2 && node.lock != LOCK_TSPIN_3 && node.lock != LOCK_PC) {
			time_waste++;
		}
	}
	if (node.lock == LOCK_BURN_1 || node.lock == LOCK_BURN_2 || node.lock == LOCK_BURN_3) {
		// if just cleared lines, but wasn't a t spin or tetris or perfect clear, then time was wasted
		time_waste++;
	}
	node.time_wasted += time_waste;
	result += node.time_wasted * w.waste_time;

	// waste piece
	result += node.T_wasted * w.waste_T;
	result += node.I_wasted * w.waste_I;

	// hold T or I
	result += (node.hold == PIECE_I) * w.hold_I + (node.hold == PIECE_T) * w.hold_T;

	return result;
}

inline void eval::column_height(bitboard& board, int result[10]) {

	for (int x = 0; x < 10; ++x) {
		int height = 20;
		for (int y = 20; y < 40; ++y) {
			if (board.is_occupying(x, y)) {
				break;
			}
			else {
				--height;
			}
		}
		result[x] = height;
	}
}

inline void eval::bumpiness(int column_height[10], int result[2])
{
	for (int i = 0; i < 9; ++i) {
		result[0] += std::abs(column_height[i] - column_height[i + 1]);
		result[1] += (column_height[i] - column_height[i + 1]) * (column_height[i] - column_height[i + 1]);
	}
}

inline void eval::blocked_cell_hole(bitboard& board, int column_height[10], int result[2])
{
	for (int i = 0; i < 10; ++i) {
		for (int k = 40 - column_height[i]; k < 40; ++k) {
			// count blocked cell
			if (!board.is_occupying(i, k))
				++result[0];

			// count hole
			if (i == 0 && !board.is_occupying(i, k) && board.is_occupying(i, k - 1) && board.is_occupying(1, k))
				++result[1];
			if (i == 9 && !board.is_occupying(i, k) && board.is_occupying(i, k - 1) && board.is_occupying(8, k))
				++result[1];
			if (i > 0 && i < 9 && !board.is_occupying(i, k) && board.is_occupying(i, k - 1) && board.is_occupying(i + 1, k) && board.is_occupying(i - 1, k))
				++result[1];
		}
	}
}

inline void eval::block_above_hole(bitboard& board, int column_height[10], int result[2])
{

	for (int i = 0; i < 10; ++i) {
		int block_count = 0;
		int hole_count = 0;
		for (int k = 40 - column_height[i]; k < 40; k++) {
			if (board.is_occupying(i, k)) {
				block_count++;
			}
			else {
				hole_count++;
				break;
			}
		}
		if (hole_count > 0) {
			result[0] += block_count;
			result[1] += block_count * block_count;
		}
	}
}

inline void eval::structure(bitboard& board, int column_height[10], int result[2])
{

	/*
	TSD
	0b100
	0b000
	0b101

	0b001
	0b000
	0b101

	STSD
	0b100
	0b000
	0b011
	0b001
	0b001

	0b001
	0b000
	0b110
	0b100
	0b100

	TST
	0b100
	0b000
	0b011
	0b001
	0b011

	0b001
	0b000
	0b110
	0b100
	0b110

	0b100
	0b000
	0b01?
	0b00?
	0b0??

	0b001
	0b000
	0b?10
	0b?00
	0b??0
	*/

	result[0] = 0;
	result[1] = 0;

	int max_height = 0;
	for (int i = 0; i < 10; ++i) {
		if (column_height[i] > max_height)
			max_height = column_height[i];
	}

	for (int x = 0; x < 8; ++x) {
	//for (int y = 40 - max_height; y < 38; ++y) {
		for (int y = 40 - max_height; y < 38; ++y) {
		//for (int x = 0; x < 8; ++x) {
			// TSD
			if ((
				((board.row[y + 0] >> (7 - x)) & 0b111) == 0b100 &&
				((board.row[y + 1] >> (7 - x)) & 0b111) == 0b000 &&
				((board.row[y + 2] >> (7 - x)) & 0b111) == 0b101 &&
				column_height[x + 1] < 40 - y && column_height[x + 2] < 40 - y
				) || (
				((board.row[y + 0] >> (7 - x)) & 0b111) == 0b001 &&
					((board.row[y + 1] >> (7 - x)) & 0b111) == 0b000 &&
					((board.row[y + 2] >> (7 - x)) & 0b111) == 0b101 &&
					column_height[x] < 40 - y && column_height[x + 1] < 40 - y
					)) {
				result[0]++;
				break;
			}

			// STSD & TST
			if (y < 36) {
				if ((
					((board.row[y + 0] >> (7 - x)) & 0b111) == 0b100 &&
					((board.row[y + 1] >> (7 - x)) & 0b111) == 0b000 &&
					((board.row[y + 2] >> (7 - x)) & 0b110) == 0b010 &&
					((board.row[y + 3] >> (7 - x)) & 0b110) == 0b000 &&
					((board.row[y + 4] >> (7 - x)) & 0b100) == 0b000 &&
					column_height[x + 1] < 40 - y && column_height[x + 2] < 40 - y
					) || (
					((board.row[y + 0] >> (7 - x)) & 0b111) == 0b001 &&
					((board.row[y + 1] >> (7 - x)) & 0b111) == 0b000 &&
					((board.row[y + 2] >> (7 - x)) & 0b011) == 0b010 &&
					((board.row[y + 3] >> (7 - x)) & 0b011) == 0b000 &&
					((board.row[y + 4] >> (7 - x)) & 0b001) == 0b000 &&
					column_height[x] < 40 - y && column_height[x + 1] < 40 - y
					)) {
					result[1]++;
					break;
				}
			}
		}
	}
}

#endif