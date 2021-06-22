#include "bot.h"

void bot::thread_start(int depth, weight w, bool memory)
{
	solution_ready = false;
	solution_need = false;
	running = true;
	this->thread = new std::thread([&](int bot_depth, weight bot_weight, bool bot_keep_memory) {
		beam a(bot_depth);
		int iter_num = 0;
		int stack_index = 0;
		int node_count = 0;
		int node_depth = 0;

		a.evaluator.w = bot_weight;

		int solution = 0b1111111111111111; // garbage random
		/*
		while (true)
		{
			a.search_one_iter(iter_num, stack_index, node_count);
			++iter_num;
			++node_depth;
			node_depth = std::min(node_depth, bot_depth);

			int latest_solution = a.get_lastest_solution();
			if (!(latest_solution == solution)) {
				solution = latest_solution;

				path_data path;
				path.set(solution);

				std::unique_lock<std::mutex> lk(mutex);
				if (solution_buffer.empty()) {
					solution_buffer.push_back(path);
				}
				else {
					solution_buffer[0] = path;
				}
			}
			std::unique_lock<std::mutex> lk(mutex);
			solution_ready = true;
			solution_buffer[0].node_count = node_count;
			solution_buffer[0].depth = node_depth;
			lk.unlock();
			cv.notify_one();

			lk.lock();
			if (!new_data_buffer.empty()) {
				a.reset(bot_keep_memory);

				iter_num = 0;
				stack_index = 0;
				node_count = 0;
				node_depth = 0;

				int solution = 0b1111111111111111; // garbage random
				a.stack[0][0].board = new_data_buffer[0].board;
				a.stack[0][0].current = new_data_buffer[0].current;
				a.stack[0][0].hold = new_data_buffer[0].hold;
				a.stack[0][0].b2b = new_data_buffer[0].b2b;
				a.stack[0][0].ren = new_data_buffer[0].ren;
				a.next_queue = new_data_buffer[0].next;
				new_data_buffer.clear();
				solution_buffer.clear();
				solution_ready = false;
			}
			if (!running) {
				break;
			}
		}
		*/

		while (true)
		{
			a.search_one_iter(iter_num, stack_index, node_count);
			++iter_num;
			++node_depth;
			node_depth = std::min(node_depth, bot_depth);

			std::unique_lock<std::mutex> lk(mutex);
			if (solution_need) {
				int latest_solution = a.get_lastest_solution();

				path_data path;
				path.set(latest_solution);

				if (solution_buffer.empty()) {
					solution_buffer.push_back(path);
				}
				else {
					solution_buffer[0] = path;
				}

				solution_need = false;
				solution_ready = true;
				solution_buffer[0].node_count = node_count;
				solution_buffer[0].depth = node_depth;
				lk.unlock();
				cv.notify_one();
				lk.lock();

			}
			if (!new_data_buffer.empty()) {
				a.reset(bot_keep_memory);

				iter_num = 0;
				stack_index = 0;
				node_count = 0;
				node_depth = 0;

				int solution = 0b1111111111111111; // garbage random
				a.stack[0][0].board = new_data_buffer[0].board;
				a.stack[0][0].current = new_data_buffer[0].current;
				a.stack[0][0].hold = new_data_buffer[0].hold;
				a.stack[0][0].b2b = new_data_buffer[0].b2b;
				a.stack[0][0].ren = new_data_buffer[0].ren;
				a.next_queue = new_data_buffer[0].next;
				new_data_buffer.clear();
				solution_buffer.clear();
				solution_ready = false;
			}
			if (!running) {
				break;
			}
		}
	}, depth, w, memory);
}

void bot::thread_destroy()
{
	std::unique_lock<std::mutex> lk(mutex);
	if (!running) {
		return;
	}
	running = false;
	lk.unlock();
	this->thread->join();
	delete this->thread;
	this->thread = nullptr;
}

path_data bot::request_solution()
{
	/*
	std::unique_lock<std::mutex> lk(mutex);
	cv.wait(lk, [&]() { return solution_ready; });
	path_data result = solution_buffer[0];
	solution_buffer.clear();
	return result;*/
	std::unique_lock<std::mutex> lk(mutex);
	solution_need = true;
	lk.unlock();
	lk.lock();
	cv.wait(lk, [&]() { return solution_ready; });
	path_data result = solution_buffer[0];
	solution_buffer.clear();
	return result;
}

void bot::set_new_data(bot_data new_data)
{
	std::unique_lock<std::mutex> lk(mutex);
	new_data_buffer.push_back(new_data);
}