#ifndef NODE_H
#define NODE_H

#include "definition.h"
#include "bitboard.h"

struct node
{
	// board data
	bitboard board;
	piece current = PIECE_NONE;
	piece hold = PIECE_NONE;
	int next = 0;
	int b2b = 0;
	int ren = 0;
	lock_data lock;

	int tspin_1 = 0;
	int tspin_2 = 0;
	int tspin_3 = 0;
	int burn_1 = 0;
	int burn_2 = 0;
	int burn_3 = 0;
	int tetris = 0;
	int pc = 0;
	int line_sent = 0;
	int max_b2b_chain = 0;
	int max_ren_chain = 0;
	int T_wasted = 0;
	int I_wasted = 0;
	int time_wasted = 0;

	int t_struct[2] = { 0, 0 };
	int p_t_struct[2] = { 0, 0 };
	int t_struct_wasted[2] = { 0, 0 };

	int score = 0;
	int depth = 0;
	vec16<int> path; // the path lead to this node

	bool operator < (const node& other) {
		return this->score < other.score;
	}

	void attempt(node& child, bool hold, const std::vector<move> & path, vec16<piece> & next_queue);
};

#endif