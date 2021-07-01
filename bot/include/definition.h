#ifndef DEFINITION_H
#define DEFINITION_H

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>

/*

#include <nlohmann/json.hpp>
using json = nlohmann::json;

*/

#include "vec.h"

enum piece {
	PIECE_NONE,
	PIECE_I,
	PIECE_J,
	PIECE_L,
	PIECE_T,
	PIECE_Z,
	PIECE_S,
	PIECE_O
};

enum move {
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_DOWN,
	MOVE_CW,
	MOVE_CCW
};

enum lock_data {
	LOCK_NONE,
	LOCK_BURN_1,
	LOCK_BURN_2,
	LOCK_BURN_3,
	LOCK_TETRIS,
	LOCK_PC,
	LOCK_TSPIN_1,
	LOCK_TSPIN_2,
	LOCK_TSPIN_3
};

struct path_data {
	piece type;
	bool is_hold;
	bool is_soft_drop;
	int index;

	void set(int other) {
		is_hold = (other & 0b1) == 0b1;
		is_soft_drop = (other & 0b10) == 0b10;
		type = (piece)((other & 0b11100) >> 2);
		index = (other >> 5) & 0b11111111;
	}

	int node_count = 0;
	int depth = 0;
};

struct weight {
	int top_height = -34;
	int top_height_top_half = -150;
	int top_height_top_quarter = -671;
	int top_height_center = -47;
	int bumpiness = -26;
	int bumpiness_s = -6;
	int well = 70;
	int hole = -176;
	int blocked_cell = -154;
	int block_above_hole = -18;
	int block_above_hole_s = -1;
	int well_1_10 = 20;
	int well_2_9 = 20;
	int well_3_8 = 20;
	int well_center = 60;
	int structure[2] = { 402, 832 };
	int b2b_max_chain = 214;
	int ren_max_chain = 218;
	int b2b_chain = 104;
	int ren_chain = 52;
	int line_sent = 57;
	int perfect_clear = 9999;
	int t_spin_1 = -167;
	int t_spin_2 = 410;
	int t_spin_3 = 683;
	int tetris = 401;
	int burn_1 = -255;
	int burn_2 = -155;
	int burn_3 = -98;
	int waste_time = -375;
	int waste_structure[2] = { -252, -355 };
	int waste_T = -156;
	int waste_I = -97;
	int hold_T = 57;
	int hold_I = 67;
};

static std::unordered_map<piece, std::vector<std::vector<move>>> path_hd{
	{PIECE_O, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }
	}},
	{PIECE_I, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }
	}},
	{PIECE_T, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }
	}},
	{PIECE_J, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }
	}},
	{PIECE_L, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CCW, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }
	}},
	{PIECE_S, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }
	}},
	{PIECE_Z, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_LEFT, MOVE_DOWN },
		{MOVE_DOWN },
		{MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN },
		{MOVE_CW, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN },
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN }

	}}
};

static std::unordered_map<piece, std::vector<std::vector<move>>> path_sd{
	{PIECE_O, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},

		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN}
	}},
	{PIECE_I, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},

		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN}
	}},
	{PIECE_T, {
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},

		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},
		{MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_CCW, MOVE_DOWN},

		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},
		{MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_CW, MOVE_DOWN},

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_CCW, MOVE_DOWN},

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_CW, MOVE_DOWN}
	}},
	{PIECE_J, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN},

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_DOWN},

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN}
	}},
	{PIECE_L, {
		{MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},
		{MOVE_LEFT, MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},
		{MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},
		{MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN},

		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_RIGHT, MOVE_DOWN},

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN}
	}},
	{PIECE_S, {
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN}
	}},
	{PIECE_Z, {
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_LEFT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},
		{MOVE_CW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CW, MOVE_DOWN},

		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_LEFT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN},
		{MOVE_CCW, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_RIGHT, MOVE_DOWN, MOVE_CCW, MOVE_DOWN}
	}}
};

static std::unordered_map<piece, std::vector<std::vector<uint16_t>>> piece_form{
	{PIECE_I, {
		{
			0b0000, 
			0b1111, 
			0b0000, 
			0b0000
		},
		{
			0b0010, 
			0b0010, 
			0b0010, 
			0b0010
		},
		{
			0b0000, 
			0b0000, 
			0b1111, 
			0b0000
		},
		{
			0b0100, 
			0b0100, 
			0b0100, 
			0b0100
		}
	}},
	{PIECE_J, {
		{
			0b1000, 
			0b1110, 
			0b0000, 
			0b0000
		},
		{
			0b0110, 
			0b0100, 
			0b0100, 
			0b0000
		},
		{
			0b0000, 
			0b1110, 
			0b0010, 
			0b0000
		},
		{
			0b0100, 
			0b0100, 
			0b1100, 
			0b0000
		}
	}},
	{PIECE_L, {
		{
			0b0010, 
			0b1110, 
			0b0000, 
			0b0000
		},
		{
			0b0100, 
			0b0100, 
			0b0110, 
			0b0000
		},
		{
			0b0000, 
			0b1110, 
			0b1000, 
			0b0000
		},
		{
			0b1100, 
			0b0100, 
			0b0100, 
			0b0000
		}
	}},
	{PIECE_S, {
		{
			0b0110, 
			0b1100, 
			0b0000, 
			0b0000
		},
		{
			0b0100, 
			0b0110, 
			0b0010, 
			0b0000
		},
		{
			0b0000, 
			0b0110, 
			0b1100, 
			0b0000
		},
		{
			0b1000, 
			0b1100, 
			0b0100, 
			0b0000
		}
	}},
	{PIECE_T, {
		{
			0b0100, 
			0b1110, 
			0b0000, 
			0b0000
		},
		{
			0b0100, 
			0b0110, 
			0b0100, 
			0b0000
		},
		{
			0b0000, 
			0b1110, 
			0b0100, 
			0b0000
		},
		{
			0b0100, 
			0b1100, 
			0b0100, 
			0b0000
		}
	}},
	{PIECE_Z, {
		{
			0b1100, 
			0b0110, 
			0b0000, 
			0b0000
		},
		{
			0b0010,
			0b0110,
			0b0100,
			0b0000
		},
		{
			0b0000, 
			0b1100,
			0b0110,
			0b0000
		},
		{
			0b0100,
			0b1100, 
			0b1000, 
			0b0000
		}
	}},
	{PIECE_O, {
		{
			0b1100,
			0b1100,
			0b0000, 
			0b0000
		}
	}}
};

static std::unordered_map<piece, int> piece_init_pos{
	{PIECE_I, 3},
	{PIECE_J, 3},
	{PIECE_L, 3},
	{PIECE_S, 3},
	{PIECE_T, 3},
	{PIECE_Z, 3},
	{PIECE_O, 4}
};

static std::unordered_map<piece, std::vector<std::vector<std::vector<int>>>> piece_srs{
	{PIECE_I, {
	{{ -2, 0 }, { 1, 0 }, { -2, 1 }, { 1, -2 }},
	{{ 2, 0 }, { -1, 0 }, { 2, -1 }, { -1, 2 }},
	{{ -1, 0 }, { 2, 0 }, { -1, -2 }, { 2, 1 }},
	{{ 1, 0 }, { -2, 0 }, { 1, 2 }, { -2, -1 }}
	}},
	{PIECE_J, {
	{{ -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
	{{ 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 }},
	{{ 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
	{{ -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 }}
	}},
	{PIECE_L, {
	{{ -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
	{{ 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 }},
	{{ 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
	{{ -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 }}
	}},
	{PIECE_S, {
	{{ -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
	{{ 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 }},
	{{ 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
	{{ -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 }}
	}},
	{PIECE_T, {
	{{ -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
	{{ 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 }},
	{{ 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
	{{ -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 }}
	}},
	{PIECE_Z, {
	{{ -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
	{{ 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 }},
	{{ 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
	{{ -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 }}
	}},
	{PIECE_O, { { {} } }}
};

#endif