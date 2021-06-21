#ifndef BEAM_H
#define BEAM_H

#include "eval.h"

class beam
{
public:
	beam(int depth, int width);
	beam(int depth);

	void reset(bool keep_memory);

	int depth;
	int width[16] = { 45, 100, 100, 125, 125, 150, 150, 150, 150, 175, 175, 175, 175, 200, 200, 200 };
	vec16<piece> next_queue;
	vec16<int> memory;
	//vec16<vec16<int>> memory;
	std::vector<std::vector<node>> stack;

	eval evaluator;

	void expand_node(node& parent, std::vector<node>& pre_beam, std::vector<node>& new_beam, const int& beam_width, const bool& is_fisrt_time, const bool& is_last_beam, int& node_count);
	void expand(std::vector<node>& pre_beam, std::vector<node>& new_beam, const int& beam_width, const bool& is_fisrt_time, const bool& is_last_beam, int& node_count);
	int search();
	void search_one_iter(int & iter_num, int & stack_index, int & node_count);
	void reconstuct_from_memory(int & iter_num, int & stack_index, int & node_count);
	int get_lastest_solution();
};

#endif