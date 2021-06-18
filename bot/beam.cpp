#include "beam.h"

beam::beam(int depth, int width)
{
	this->depth = depth;
	this->width[0] = width;
	this->stack.reserve(this->depth + 1);
	for (int i = 0; i < this->depth + 1; i++) {
		this->stack.push_back(std::vector<node>());
		this->stack[i].reserve(16000);
	}
	node root;
	this->stack[0].push_back(root);
}

beam::beam(int depth)
{
	this->depth = depth;
	this->stack.reserve(this->depth + 1);
	for (int i = 0; i < this->depth + 1; i++) {
		this->stack.push_back(std::vector<node>());
		this->stack[i].reserve(16000);
	}
	node root;
	this->stack[0].push_back(root);
}

/*
Reset the beam while keeping the path led to the best node in previous search
Maybe this will improve beam search? idk
*/
void beam::reset(bool keep_memory)
{
	if (keep_memory) {
		int index = depth;
		while (index > 0 && stack[index].size() < 1) { --index; }

		if (index == 0) {
			memory.clear();
		}
		else {
			memory = stack[index][0].path;
			memory.erase(0);
		}
	}
	
	this->stack.clear();
	this->stack.reserve(this->depth + 1);
	for (int i = 0; i < this->depth + 1; i++) {
		this->stack.push_back(std::vector<node>());
		this->stack[i].reserve(16000);
	}
	node root;
	this->stack[0].push_back(root);
}

void beam::expand_node(node & parent, std::vector<node>& pre_beam, std::vector<node>& new_beam, const int & beam_width, const bool & is_fisrt_time, const bool & is_last_beam, int & node_count)
{
	if (parent.current == PIECE_NONE)
		return;

	vec128<bitboard> hard_drop_current;
	vec128<bitboard> hard_drop_hold;

	// Hard drop
	for (int i = 0; i < (int)path_hd[parent.current].size(); ++i) {
		node child;
		parent.attempt(child, false, path_hd[parent.current][i], next_queue);
		//child.path = parent.path;
		child.path.add((0b00 | (parent.current << 2)) | ((i & 0b11111111) << 5));
		child.score = evaluator.evaluate(child); // replace with a good evaluator

		hard_drop_current.add(child.board);

		if (is_last_beam) {
			if ((int)new_beam.size() < 1) new_beam.push_back(child);
			if (child.score > new_beam[0].score) new_beam[0] = child;
		}
		else {
			new_beam.push_back(child);
			std::push_heap(new_beam.begin(), new_beam.end());
		}
		++node_count;
	};

	if (parent.hold == PIECE_NONE) {
		if (parent.next < next_queue.size) {
			for (int i = 0; i < (int)path_hd[next_queue[parent.next]].size(); ++i) {
				node child;
				parent.attempt(child, true, path_hd[next_queue[parent.next]][i], next_queue);
				//child.path = parent.path;
				child.path.add((0b01 | (next_queue[parent.next] << 2)) | ((i & 0b11111111) << 5));
				child.score = evaluator.evaluate(child); // replace with a good evaluator

				hard_drop_hold.add(child.board);

				if (is_last_beam) {
					if ((int)new_beam.size() < 1) new_beam.push_back(child);
					if (child.score > new_beam[0].score) new_beam[0] = child;
				}
				else {
					new_beam.push_back(child);
					std::push_heap(new_beam.begin(), new_beam.end());
				}
				++node_count;
			};
		}
	}
	else {
		for (int i = 0; i < (int)path_hd[parent.hold].size(); ++i) {
			node child;
			parent.attempt(child, true, path_hd[parent.hold][i], next_queue);
			//child.path = parent.path;
			child.path.add((0b01 | (parent.hold << 2)) | ((i & 0b11111111) << 5));
			child.score = evaluator.evaluate(child); // replace with a good evaluator

			hard_drop_hold.add(child.board);

			if (is_last_beam) {
				if ((int)new_beam.size() < 1) new_beam.push_back(child);
				if (child.score > new_beam[0].score) new_beam[0] = child;
			}
			else {
				new_beam.push_back(child);
				std::push_heap(new_beam.begin(), new_beam.end());
			}
			++node_count;
		};
	}

	// Soft drop
	for (int i = 0; i < (int)path_sd[parent.current].size(); ++i) {
		node child;
		parent.attempt(child, false, path_sd[parent.current][i], next_queue);
		//child.path = parent.path;
		child.path.add((0b10 | (parent.current << 2)) | ((i & 0b11111111) << 5));

		// check if existing
		bool existed = false;
		for (int i = 0; i < hard_drop_current.size; ++i) {
			if (child.board == hard_drop_current[i]) {
				existed = true;
				break;
			}
		}

		if (!existed) {
			child.score = evaluator.evaluate(child); // replace with a good evaluator

			hard_drop_current.add(child.board);

			if (is_last_beam) {
				if ((int)new_beam.size() < 1) new_beam.push_back(child);
				if (child.score > new_beam[0].score) new_beam[0] = child;
			}
			else {
				new_beam.push_back(child);
				std::push_heap(new_beam.begin(), new_beam.end());
			}
			++node_count;
		}
	};

	if (parent.hold == PIECE_NONE) {
		if (parent.next < next_queue.size) {
			for (int i = 0; i < (int)path_sd[next_queue[parent.next]].size(); ++i) {
				node child;
				parent.attempt(child, true, path_sd[next_queue[parent.next]][i], next_queue);
				//child.path = parent.path;
				child.path.add((0b11 | (next_queue[parent.next] << 2)) | ((i & 0b11111111) << 5));

				// check if existing
				bool existed = false;
				for (int i = 0; i < hard_drop_hold.size; ++i) {
					if (child.board == hard_drop_hold[i]) {
						existed = true;
						break;
					}
				}

				if (!existed) {
					child.score = evaluator.evaluate(child); // replace with a good evaluator

					hard_drop_hold.add(child.board);

					if (is_last_beam) {
						if ((int)new_beam.size() < 1) new_beam.push_back(child);
						if (child.score > new_beam[0].score) new_beam[0] = child;
					}
					else {
						new_beam.push_back(child);
						std::push_heap(new_beam.begin(), new_beam.end());
					}
					++node_count;
				}
			};
		}
	}
	else {
		for (int i = 0; i < (int)path_sd[parent.hold].size(); ++i) {
			node child;
			parent.attempt(child, true, path_sd[parent.hold][i], next_queue);
			//child.path = parent.path;
			child.path.add((0b11 | (parent.hold << 2)) | ((i & 0b11111111) << 5));

			// check if existing
			bool existed = false;
			for (int i = 0; i < hard_drop_hold.size; ++i) {
				if (child.board == hard_drop_hold[i]) {
					existed = true;
					break;
				}
			}

			if (!existed) {
				child.score = this->evaluator.evaluate(child); // replace with a good evaluator

				hard_drop_hold.add(child.board);

				if (is_last_beam) {
					if ((int)new_beam.size() < 1) new_beam.push_back(child);
					if (child.score > new_beam[0].score) new_beam[0] = child;
				}
				else {
					new_beam.push_back(child);
					std::push_heap(new_beam.begin(), new_beam.end());
				}
				++node_count;
			}
		};
	}
}

void beam::expand(std::vector<node>& pre_beam, std::vector<node>& new_beam, const int & beam_width, const bool & is_fisrt_time, const bool & is_last_beam, int & node_count)
{
	// expand it by pop heap
	for (int i = 0; i < beam_width; ++i) {
		if (pre_beam.size() < 1) {
			break;
		}
		expand_node(pre_beam[0], pre_beam, new_beam, beam_width, is_fisrt_time, is_last_beam, node_count);

		std::pop_heap(pre_beam.begin(), pre_beam.end());
		pre_beam.pop_back();
	}
}

void beam::reconstuct_from_memory(int & iter_num, int & stack_index, int & node_count)
{
	if (memory.size > 0) {
		node rebuild_node = stack[0][0];
		for (int i = 0; i < memory.size; ++i) {
			node temp_node;
			if ((memory[i] & 0b10) == 0b10) { // if soft drop
				rebuild_node.attempt(temp_node, ((memory[i] & 0b1) == 0b1), path_sd[(piece)((memory[i] & 0b11100) >> 2)][(memory[i] >> 5) & 0b11111111], next_queue);
			}
			else {
				rebuild_node.attempt(temp_node, ((memory[i] & 0b1) == 0b1), path_hd[(piece)((memory[i] & 0b11100) >> 2)][(memory[i] >> 5) & 0b11111111], next_queue);
			}
			rebuild_node = temp_node;
		}
		rebuild_node.path = memory;
		stack[memory.size].push_back(rebuild_node);
		expand(stack[memory.size], stack[memory.size + 1], 1, false, (memory.size == depth - 1), node_count);
	}
}

int beam::search()
{
	int node_count = 0;
	int iter_num = 0;
	int stack_index = 0;

	while (iter_num < 20)
	{
		this->search_one_iter(iter_num, stack_index, node_count);
		++iter_num;
	}

	return node_count;
}

void beam::search_one_iter(int & iter_num, int & stack_index, int & node_count)
{
	int s_width = width[0];

	if (iter_num == 0) {
		// forced search from memory
		reconstuct_from_memory(iter_num, stack_index, node_count);

		// level 1 search
		expand(stack[0], stack[1], 1, true, false, node_count);
		stack_index = 0;
	}
	else {
		if (iter_num < depth) {
			// normal beam search
			stack_index = iter_num;
			s_width = width[iter_num - 1];
		}
		else {
			// backtrack
			s_width = width[0];
			stack_index++;
			if (stack_index >= depth) {
				stack_index = depth - 1;
				while (stack[stack_index].size() < 1)
				{
					if (stack_index == 0)
						break;
					stack_index--;
				}
				if (stack_index == 0)
					return;
			}
		}

		expand(this->stack[stack_index], this->stack[stack_index + 1], s_width, false, (stack_index == depth - 1), node_count);
	}
}

int beam::get_lastest_solution()
{
	int index = depth;
	while (index > 0 && stack[index].empty())
	{
		--index;
	}

	if (index == 0) {
		return 0;
	}
	else {
		return stack[index][0].path[0];
	}
}
