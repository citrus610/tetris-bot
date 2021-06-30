#include "game_bot.h"

void game_bot::load()
{
	srand(time(NULL));
	this->img_counter.loadFromFile("res/graphic/counter.png");
	this->sp_counter.setTexture(this->img_counter);
	this->board_1.set_enemy(&this->board_2);
	this->board_2.set_enemy(&this->board_1);
	this->board_1.init();
	this->board_2.init();
	this->gameover_counter = 119;

	create_init_js();
	std::ifstream file;
	file.open("init.json");
	json js;
	file >> js;
	set_weight_js(this->w_1, js, 1);
	set_weight_js(this->w_2, js, 2);
	file.close();

	if (this->enable_bot_1) { 
		this->board_1.enable_bot(); 
	}
	if (this->enable_bot_2) { 
		this->board_2.enable_bot(); 
	}
}

void game_bot::update(double dt)
{
	this->gameover_counter++;

	if (this->gameover_counter == 120)
		this->start_game();

	if (this->gameover_counter >= 300) {
		this->gameover_counter = 301;
		this->update_game(dt);
		if (this->board_1.is_game_over() || this->board_2.is_game_over() || input::x.justPressed()) {
			this->gameover_counter = 0;
			this->end_game();
		}
	}
}

void game_bot::render()
{
	this->canvas.clear();
	this->canvas.setSmooth(true);
	this->board_1.render(&this->canvas);
	this->board_2.render(&this->canvas);

	this->sp_counter.setOrigin(10, 15);
	this->sp_counter.setPosition(960, 525);
	this->sp_counter.setScale(4, 4);
	if (this->gameover_counter > 120) {
		if (this->gameover_counter < 180) {
			this->sp_counter.setTextureRect(sf::IntRect(40, 0, 20, 30));
			this->canvas.draw(this->sp_counter);
		}
		else if (this->gameover_counter < 240) {
			this->sp_counter.setTextureRect(sf::IntRect(20, 0, 20, 30));
			this->canvas.draw(this->sp_counter);
		}
		else if (this->gameover_counter < 300) {
			this->sp_counter.setTextureRect(sf::IntRect(0, 0, 20, 30));
			this->canvas.draw(this->sp_counter);
		}
	}
}

void game_bot::unload()
{
	if (this->enable_bot_1) this->bot_1.thread_destroy();
	if (this->enable_bot_2) this->bot_2.thread_destroy();
}

void game_bot::start_game()
{
	this->board_1.init();
	this->board_2.init();

	if (this->enable_bot_1) {
		this->bot_1.thread_start(5, this->w_1, true);
		bot_data data_1 = board_to_bot_data(this->board_1);
		this->bot_1.set_new_data(data_1);
	}

	if (this->enable_bot_2) {
		this->bot_2.thread_start(5, this->w_2, true);
		bot_data data_2 = board_to_bot_data(this->board_2);
		this->bot_2.set_new_data(data_2);
	}
}

void game_bot::update_game(double dt)
{
	if (this->enable_bot_1) this->handle_bot_input(dt, 1, this->board_1, this->bot_1, this->bot_input_timer_1, this->solution_1);
	if (this->enable_bot_2) this->handle_bot_input(dt, 2, this->board_2, this->bot_2, this->bot_input_timer_2, this->solution_2);
	/*if (!this->board_1.is_clearing_line() && !this->board_1.is_placing_piece()) {
		if (this->solution_1.empty()) {
			path_data solution = this->bot_1.request_solution();
			if (solution.is_hold) this->board_1.hold();
			if (solution.is_soft_drop)
				this->solution_1 = path_sd[solution.type][solution.index]; // out of range
			else
				this->solution_1 = path_hd[solution.type][solution.index]; // out of range

			vec16<piece> next_q;
			next_q.size = 0;
			for (int i = 0; i < (int)this->board_1.next_piece.size(); ++i)
				next_q.add(char_to_piece(this->board_1.next_piece[i]));

			node old_n, new_n;

			//old_n = board_to_node(_board);
			old_n.current = char_to_piece(this->board_1.current_piece);
			old_n.hold = char_to_piece(this->board_1.hold_piece);
			old_n.b2b = this->board_1.b2b;
			old_n.ren = this->board_1.ren;
			old_n.next = 0;
			for (int i = 0; i < 40; ++i) {
				uint16_t a_row = 0b0;
				for (int k = 0; k < 10; ++k) {
					if (this->board_1.data[i][k] > 0) {
						a_row = a_row | (0b1 << (9 - k));
					}
				}
				old_n.board.row[i] = a_row;
			}

			old_n.attempt(new_n, false, this->solution_1, next_q);
			next_q.erase(0);
			bot_data new_data{ new_n.board, new_n.current, new_n.hold, next_q, new_n.b2b, new_n.ren };

			this->bot_1.set_new_data(new_data);

			this->bot_input_timer_1 = 0.0;
			this->board_1.bot_node = solution.node_count;
			this->board_1.bot_depth = solution.depth;
		}
		else {
			this->bot_input_timer_1 += dt;
			switch (this->solution_1[0])
			{
			case MOVE_CW:
				this->board_1.real_piece.try_rotate(this->board_1.data, 1);
				this->bot_input_timer_1 = 0.0;
				this->solution_1.erase(this->solution_1.begin() + 0);
				break;
			case MOVE_CCW:
				this->board_1.real_piece.try_rotate(this->board_1.data, 3);
				this->bot_input_timer_1 = 0.0;
				this->solution_1.erase(this->solution_1.begin() + 0);
				break;
			case MOVE_LEFT:
				if (this->bot_input_timer_1 >= this->bot_input_delay) {
					this->board_1.real_piece.try_left(this->board_1.data);
					this->bot_input_timer_1 = 0.0;
					this->solution_1.erase(this->solution_1.begin() + 0);
				}
				break;
			case MOVE_RIGHT:
				if (this->bot_input_timer_1 >= this->bot_input_delay) {
					this->board_1.real_piece.try_right(this->board_1.data);
					this->bot_input_timer_1 = 0.0;
					this->solution_1.erase(this->solution_1.begin() + 0);
				}
				break;
			case MOVE_DOWN:
				if (this->solution_1.size() == 1) {
					if (this->bot_input_timer_1 >= this->bot_input_delay) {
						this->board_1.real_piece.try_hard_drop(this->board_1.data);
						this->bot_input_timer_1 = 0.0;
						this->solution_1.erase(this->solution_1.begin() + 0);
					}
				}
				else {
					if (this->bot_input_timer_1 >= this->bot_input_delay) {
						this->board_1.real_piece.try_down(this->board_1.data);
						this->bot_input_timer_1 = 0.0;
						if (this->board_1.real_piece.is_colliding(this->board_1.data, this->board_1.real_piece.x, this->board_1.real_piece.y + 1)) this->solution_1.erase(this->solution_1.begin() + 0);
					}
				}
				break;
			default:
				break;
			}
		}
	}*/

	this->board_1.update(dt);
	this->board_2.update(dt);
}

void game_bot::end_game()
{
	this->solution_1.clear();
	this->bot_input_timer_1 = 0.0;
	if (this->enable_bot_1) this->bot_1.thread_destroy();

	this->solution_2.clear();
	this->bot_input_timer_2 = 0.0;
	if (this->enable_bot_2) this->bot_2.thread_destroy();
}

void game_bot::handle_bot_input(double dt, int id, tetris_board & _board, bot & _bot, double & bot_input_timer, std::vector<move>& solution_vec)
{
	if (!_board.is_clearing_line() && !_board.is_placing_piece()) {
		if (solution_vec.empty()) {
			path_data solution = _bot.request_solution();
			if (solution.is_hold) _board.hold();
			if (solution.is_soft_drop)
				solution_vec = path_sd[solution.type][solution.index]; // out of range
			else
				solution_vec = path_hd[solution.type][solution.index]; // out of range

			vec16<piece> next_q;
			next_q.size = 0;
			for (auto & p : _board.next_piece)
				next_q.add(char_to_piece(p));

			node old_n; 
			node new_n;

			old_n = board_to_node(_board);
			
			old_n.attempt(new_n, false, solution_vec, next_q);
			next_q.erase(0);
			bot_data new_data{ new_n.board, new_n.current, new_n.hold, next_q, new_n.b2b, new_n.ren };

			_bot.set_new_data(new_data);

			bot_input_timer = 0.0;
			_board.bot_node = solution.node_count;
			_board.bot_depth = solution.depth;
		}

		if (!solution_vec.empty()) {
			bot_input_timer += dt;
			switch (solution_vec[0])
			{
			case MOVE_CW:
				_board.real_piece.try_rotate(_board.data, 1);
				bot_input_timer = 0.0;
				solution_vec.erase(solution_vec.begin() + 0);
				break;
			case MOVE_CCW:
				_board.real_piece.try_rotate(_board.data, 3);
				bot_input_timer = 0.0;
				solution_vec.erase(solution_vec.begin() + 0);
				break;
			case MOVE_LEFT:
				if (bot_input_timer >= this->bot_input_delay) {
					_board.real_piece.try_left(_board.data);
					bot_input_timer = 0.0;
					solution_vec.erase(solution_vec.begin() + 0);
				}
				break;
			case MOVE_RIGHT:
				if (bot_input_timer >= this->bot_input_delay) {
					_board.real_piece.try_right(_board.data);
					bot_input_timer = 0.0;
					solution_vec.erase(solution_vec.begin() + 0);
				}
				break;
			case MOVE_DOWN:
				if (solution_vec.size() == 1) {
					if (bot_input_timer >= this->bot_input_delay) {
						_board.real_piece.try_hard_drop(_board.data);
						bot_input_timer = 0.0;
						solution_vec.clear();
					}
				}
				else {
					if (bot_input_timer >= this->bot_input_delay) {
						_board.real_piece.try_down(_board.data);
						bot_input_timer = 0.0;
						if (_board.real_piece.is_colliding(_board.data, _board.real_piece.x, _board.real_piece.y + 1)) solution_vec.erase(solution_vec.begin() + 0);
					}
				}
				break;
			default:
				break;
			}
		}
	}
}
