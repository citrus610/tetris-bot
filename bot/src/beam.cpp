#include "beam.h"

beam::beam(int depth)
{
	this->depth = depth;
	this->stack.reserve(this->depth + 1);
	for (int i = 0; i < this->depth + 1; i++) {
		this->stack.push_back(std::vector<node>());
		if (i != 0 && i != this->depth) 
			this->stack[i].reserve(beam::max_mem);
		else
			this->stack[i].reserve(1);
		//this->stack[i].reserve(16000);
	}
	node root;
	this->stack[0].push_back(root);
	this->path_reward.clear();
}

void beam::reset()
{
	for (int i = 0; i < this->depth + 1; ++i) {
		this->stack[i].clear();
	}
	node root;
	this->stack[0].push_back(root);
	this->path_reward.clear();
}

void beam::expand_node(node & parent, const int & pre_stack_index, const int & new_stack_index, const int & beam_width, const bool & is_first_iter, const bool & is_last_stack, int & node_count)
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
		//child.path.add((0b00 | (parent.current << 2)) | ((i & 0b11111111) << 5));
		child.path = (0b00 | (parent.current << 2)) | ((i & 0b11111111) << 5);
		child.score = evaluator.evaluate(child); // replace with a good evaluator
		if (is_first_iter) {
			child.org_path = child.path;
			path_reward[child.org_path] = reward{1, child.score};
		}

		hard_drop_current.add(child.board);

		if (is_last_stack) {
			if (stack[new_stack_index].empty()) stack[new_stack_index].push_back(child);
			if (child.score > stack[new_stack_index][0].score) stack[new_stack_index][0] = child;
			//++path_reward[stack[new_stack_index][0].org_path].visit;
		}
		else {
			stack[new_stack_index].push_back(std::move(child));
			std::push_heap(stack[new_stack_index].begin(), stack[new_stack_index].end());
		}
		++node_count;
	};

	if (parent.hold == PIECE_NONE) {
		if (parent.next < next_queue.size) {
			for (int i = 0; i < (int)path_hd[next_queue[parent.next]].size(); ++i) {
				node child;
				parent.attempt(child, true, path_hd[next_queue[parent.next]][i], next_queue);
				//child.path = parent.path;
				//child.path.add((0b01 | (next_queue[parent.next] << 2)) | ((i & 0b11111111) << 5));
				child.path = (0b01 | (next_queue[parent.next] << 2)) | ((i & 0b11111111) << 5);
				child.score = evaluator.evaluate(child); // replace with a good evaluator
				if (is_first_iter) {
					child.org_path = child.path;
					path_reward[child.org_path] = reward{ 1, child.score };
				}

				hard_drop_hold.add(child.board);

				if (is_last_stack) {
					if (stack[new_stack_index].empty()) stack[new_stack_index].push_back(child);
					if (child.score > stack[new_stack_index][0].score) stack[new_stack_index][0] = child;
					//++path_reward[stack[new_stack_index][0].org_path].visit;
				}
				else {
					stack[new_stack_index].push_back(std::move(child));
					std::push_heap(stack[new_stack_index].begin(), stack[new_stack_index].end());
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
			//child.path.add((0b01 | (parent.hold << 2)) | ((i & 0b11111111) << 5));
			child.path = (0b01 | (parent.hold << 2)) | ((i & 0b11111111) << 5);
			child.score = evaluator.evaluate(child); // replace with a good evaluator
			if (is_first_iter) {
				child.org_path = child.path;
				path_reward[child.org_path] = reward{ 1, child.score };
			}

			hard_drop_hold.add(child.board);

			if (is_last_stack) {
				if (stack[new_stack_index].empty()) stack[new_stack_index].push_back(child);
				if (child.score > stack[new_stack_index][0].score) stack[new_stack_index][0] = child;
				//++path_reward[stack[new_stack_index][0].org_path].visit;
			}
			else {
				stack[new_stack_index].push_back(std::move(child));
				std::push_heap(stack[new_stack_index].begin(), stack[new_stack_index].end());
			}
			++node_count;
		};
	}

	// Soft drop
	for (int i = 0; i < (int)path_sd[parent.current].size(); ++i) {
		node child;
		parent.attempt(child, false, path_sd[parent.current][i], next_queue);
		//child.path = parent.path;
		//child.path.add((0b10 | (parent.current << 2)) | ((i & 0b11111111) << 5));
		child.path = (0b10 | (parent.current << 2)) | ((i & 0b11111111) << 5);

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
			if (is_first_iter) {
				child.org_path = child.path;
				path_reward[child.org_path] = reward{ 1, child.score };
			}

			hard_drop_current.add(child.board);

			if (is_last_stack) {
				if (stack[new_stack_index].empty()) stack[new_stack_index].push_back(child);
				if (child.score > stack[new_stack_index][0].score) stack[new_stack_index][0] = child;
				//++path_reward[stack[new_stack_index][0].org_path].visit;
			}
			else {
				stack[new_stack_index].push_back(std::move(child));
				std::push_heap(stack[new_stack_index].begin(), stack[new_stack_index].end());
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
				//child.path.add((0b11 | (next_queue[parent.next] << 2)) | ((i & 0b11111111) << 5));
				child.path = (0b11 | (next_queue[parent.next] << 2)) | ((i & 0b11111111) << 5);

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
					if (is_first_iter) {
						child.org_path = child.path;
						path_reward[child.org_path] = reward{ 1, child.score };
					}

					hard_drop_hold.add(child.board);

					if (is_last_stack) {
						if (stack[new_stack_index].empty()) stack[new_stack_index].push_back(child);
						if (child.score > stack[new_stack_index][0].score) stack[new_stack_index][0] = child;
						//++path_reward[stack[new_stack_index][0].org_path].visit;
					}
					else {
						stack[new_stack_index].push_back(std::move(child));
						std::push_heap(stack[new_stack_index].begin(), stack[new_stack_index].end());
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
			//child.path.add((0b11 | (parent.hold << 2)) | ((i & 0b11111111) << 5));
			child.path = (0b11 | (parent.hold << 2)) | ((i & 0b11111111) << 5);

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
				if (is_first_iter) {
					child.org_path = child.path;
					path_reward[child.org_path] = reward{ 1, child.score };
				}

				hard_drop_hold.add(child.board);

				if (is_last_stack) {
					if (stack[new_stack_index].empty()) stack[new_stack_index].push_back(child);
					if (child.score > stack[new_stack_index][0].score) stack[new_stack_index][0] = child;
					//++path_reward[stack[new_stack_index][0].org_path].visit;
				}
				else {
					stack[new_stack_index].push_back(std::move(child));
					std::push_heap(stack[new_stack_index].begin(), stack[new_stack_index].end());
				}
				++node_count;
			}
		};
	}
}

void beam::expand(const int & pre_stack_index, const int & new_stack_index, const int & beam_width, const bool & is_first_iter, const bool & is_last_stack, int & node_count)
{
	// expand it by pop heap
	for (int i = 0; i < beam_width; ++i) {
		if (stack[pre_stack_index].empty()) break;
		expand_node(stack[pre_stack_index][0], pre_stack_index, new_stack_index, beam_width, is_first_iter, is_last_stack, node_count);

		// add reward
		if (!is_first_iter) ++path_reward[stack[pre_stack_index][0].org_path].visit;
		if (!stack[depth].empty()) ++path_reward[stack[depth][0].org_path].visit;

		std::pop_heap(stack[pre_stack_index].begin(), stack[pre_stack_index].end());
		stack[pre_stack_index].pop_back();
	}

	// discard the worst nodes since they are not worth checking
	if ((int)stack[pre_stack_index].size() < prune) stack[pre_stack_index].clear();
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
		// force level 1 search
		expand(0, 1, 1, true, false, node_count);
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

		expand(stack_index, stack_index + 1, s_width, false, (stack_index == depth - 1), node_count);
	}
}

int beam::get_lastest_solution()
{
	std::pair<int, reward> best_pair = std::make_pair(0, reward{-99999999, -99999999});
	std::unordered_map<int, reward>::iterator current_pair;
	for (current_pair = path_reward.begin(); current_pair != path_reward.end(); ++current_pair) {
		if (current_pair->second > best_pair.second) {
			best_pair = std::make_pair(current_pair->first, current_pair->second);
		}
	}
	return best_pair.first;
}