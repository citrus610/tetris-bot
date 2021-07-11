#ifndef BOT_H
#define BOT_H

#include "beam.h"

struct bot_data {
	bitboard board;
	piece current;
	piece hold;
	vec16<piece> next;
	int b2b;
	int ren;
};

class bot
{
	//public:
	std::mutex mutex;
	std::condition_variable cv;
	bool solution_ready = false, solution_need = false, running = true; // Set before starting std::thread
	std::vector<bot_data> new_data_buffer;
	std::vector<path_data> solution_buffer;

public:
	std::thread * thread;

	void thread_start(int depth, weight weight);
	void thread_destroy();
	path_data request_solution();
	void set_new_data(bot_data new_data);
};

#endif