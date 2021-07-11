#include "bot.h"

void bot::thread_start(int depth, weight w)
{
	solution_ready = false;
	solution_need = false;
	running = true;

	this->thread = new std::thread([&](int bot_depth, weight bot_weight) {
		beam bot_beam(bot_depth);
		int iter_num = 0;
		int stack_index = 0;
		int node_count = 0;
		int node_depth = 0;

		bot_beam.evaluator.w = bot_weight;

		while (true)
		{
			bot_beam.search_one_iter(iter_num, stack_index, node_count);
			++iter_num;
			++node_depth;
			node_depth = std::min(node_depth, bot_depth);

			std::unique_lock<std::mutex> lk(mutex);
			if (solution_need) {
				solution_need = false;

				path_data new_solution;
				new_solution.set(bot_beam.get_lastest_solution());
				new_solution.node_count = node_count;
				new_solution.depth = node_depth;

				solution_buffer.clear();
				solution_buffer.push_back(new_solution);
				solution_ready = true;
				lk.unlock();
				cv.notify_one();
				lk.lock();
			}
			if (!new_data_buffer.empty()) {
				bot_beam.reset();

				iter_num = 0;
				stack_index = 0;
				node_count = 0;
				node_depth = 0;

				bot_beam.stack[0][0].board = new_data_buffer[0].board;
				bot_beam.stack[0][0].current = new_data_buffer[0].current;
				bot_beam.stack[0][0].hold = new_data_buffer[0].hold;
				bot_beam.stack[0][0].b2b = new_data_buffer[0].b2b;
				bot_beam.stack[0][0].ren = new_data_buffer[0].ren;
				bot_beam.stack[0][0].next = 0;
				bot_beam.next_queue = new_data_buffer[0].next;
				new_data_buffer.clear();
				solution_buffer.clear();
				solution_ready = false;
			}
			if (!running) {
				break;
			}
		}

	}, depth, w);
}

void bot::thread_destroy()
{
	std::unique_lock<std::mutex> lk(mutex);
	if (!running) return;
	running = false;
	solution_ready = false;
	solution_need = false;
	lk.unlock();
	this->thread->join();
	delete this->thread;
	this->thread = nullptr;
}

path_data bot::request_solution()
{
	std::unique_lock<std::mutex> lk(mutex);
	solution_need = true;
	lk.unlock();

	lk.lock();
	cv.wait(lk, [&]() { return solution_ready; });
	path_data result = solution_buffer[0];
	solution_buffer.clear();
	solution_ready = false;
	return result;
}

void bot::set_new_data(bot_data new_data)
{
	std::unique_lock<std::mutex> lk(mutex);
	new_data_buffer.push_back(new_data);
}