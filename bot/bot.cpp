#include "bot.h"

void bot::thread_start()
{
	solution_ready = false;
	running = true;
	this->thread = new std::thread([&]() {
		beam a(7);
		int iter_num = 0;
		int stack_index = 0;
		int node_count = 0;

		a.evaluator.w.set("init.json");

		int solution = 0b1111111111111111;

		while (true)
		{
			a.search_one_iter(iter_num, stack_index, node_count);
			++iter_num;

			int latest_solution = a.get_lastest_solution();
			if (!(latest_solution == solution)) {
				solution = latest_solution;
				std::unique_lock<std::mutex> lk(mutex);
				if (solution_buffer.empty()) {
					solution_buffer.push_back(solution);
				}
				else {
					solution_buffer[0] = solution;
				}
			}
			std::unique_lock<std::mutex> lk(mutex);
			solution_ready = true;
			lk.unlock();
			cv.notify_one();

			lk.lock();
			if (!new_data_buffer.empty()) {
				a.reset(true);

				/*std::cout << "node: " << node_count << std::endl;
				for (int i = a.depth; i >= 0; --i) {
					if ((int)a.stack[i].size() > 0) {
						std::cout << "depth: " << (a.depth - i) << std::endl;
						break;
					}
				}*/
				iter_num = 0;
				stack_index = 0;
				node_count = 0;

				solution = 0b1111111111111111;
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
	});
}

void bot::thread_start(int depth, weight w)
{
	solution_ready = false;
	running = true;
	this->thread = new std::thread([&](int bot_depth, weight bot_weight) {
		beam a(bot_depth);
		int iter_num = 0;
		int stack_index = 0;
		int node_count = 0;

		a.evaluator.w = bot_weight;

		int solution = 0b1111111111111111;

		while (true)
		{
			a.search_one_iter(iter_num, stack_index, node_count);
			++iter_num;

			int latest_solution = a.get_lastest_solution();
			if (!(latest_solution == solution)) {
				solution = latest_solution;
				std::unique_lock<std::mutex> lk(mutex);
				if (solution_buffer.empty()) {
					solution_buffer.push_back(solution);
				}
				else {
					solution_buffer[0] = solution;
				}
			}
			std::unique_lock<std::mutex> lk(mutex);
			solution_ready = true;
			lk.unlock();
			cv.notify_one();

			lk.lock();
			if (!new_data_buffer.empty()) {
				a.reset(true);

				/*std::cout << "node: " << node_count << std::endl;
				for (int i = a.depth; i >= 0; --i) {
					if ((int)a.stack[i].size() > 0) {
						std::cout << "depth: " << (a.depth - i) << std::endl;
						break;
					}
				}*/
				iter_num = 0;
				stack_index = 0;
				node_count = 0;

				int solution = 0b1111111111111111;
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
	}, depth, w);
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
	std::unique_lock<std::mutex> lk(mutex);
	cv.wait(lk, [&]() { return solution_ready; });
	int result_raw = solution_buffer[0];
	solution_buffer.clear();
	path_data result;
	result.set(result_raw);
	return result;
}

void bot::set_new_data(bot_data new_data)
{
	std::unique_lock<std::mutex> lk(mutex);
	new_data_buffer.push_back(new_data);
}