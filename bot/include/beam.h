/*
This is the beam class, the heart of the bot.
*/

#ifndef BEAM_H
#define BEAM_H

#include "eval.h"

class beam
{
public:
	beam(int depth);

	int depth;
	int width[16] = { 50, 150, 150, 150, 150, 150, 150, 150, 150, 175, 175, 175, 175, 200, 200, 200 };
	const size_t max_mem = 16000, prune = 5000;

	vec16<piece> next_queue;
	std::vector<std::vector<node>> stack;
	std::unordered_map<int, reward> path_reward;

	eval evaluator;

	void expand_node(node& parent, const int & pre_stack_index, const int & new_stack_index, const int & beam_width, const bool & is_first_iter, const bool & is_last_stack, int & node_count);
	void expand(const int & pre_stack_index, const int & new_stack_index, const int & beam_width, const bool & is_first_iter, const bool & is_last_stack, int & node_count);

	void reset();
	int search();
	void search_one_iter(int & iter_num, int & stack_index, int & node_count);
	int get_lastest_solution();
};

#endif