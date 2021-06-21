#include "Game1.h"
#include <bitset>

void Game1::load()
{
	this->image_of_number.loadFromFile("Content/Graphic/Counter.png");
	this->image_of_number.setSmooth(false);
	this->sprite_image_of_number.setTexture(this->image_of_number);

	this->board1.setPosScale(80, 140, 40);
	this->board2.setPosScale(80 + 1920 / 2, 140, 40);
	this->board1.setEnemy(&this->board2);
	this->board2.setEnemy(&this->board1);
	this->board1.init();
	this->board2.init();

	this->gameover_timer = 4.0f;
	this->new_game_timer = 4.0f;

	std::ifstream file;
	file.open("init.json");
	json js;
	file >> js;
	set_weight_js(this->w_1, js, 1);
	set_weight_js(this->w_2, js, 2);
	file.close();

	if (this->is_1_bot) {
		this->board1.enableAI();
		///*
		this->bot_1.thread_start(5, this->w_1, true);

		bot_data data_1;
		data_1.current = char_to_piece(this->board1.currentPiece);
		data_1.hold = PIECE_NONE;
		data_1.b2b = false;
		data_1.ren = 0;
		for (auto & p : this->board1.nextPiece) {
			data_1.next.add(char_to_piece(p));
		}
		for (int i = 0; i < 40; ++i) {
			data_1.board.row[i] = 0b0;
		}

		this->bot_1.set_new_data(data_1);
	}

	if (this->is_2_bot) {
		this->board2.enableAI();
		///*
		this->bot_2.thread_start(5, this->w_2, true);

		bot_data data_2;
		data_2.current = char_to_piece(this->board2.currentPiece);
		data_2.hold = PIECE_NONE;
		data_2.b2b = false;
		data_2.ren = 0;
		for (auto & p : this->board2.nextPiece) {
			data_2.next.add(char_to_piece(p));
		}
		for (int i = 0; i < 40; ++i) {
			data_2.board.row[i] = 0b0;
		}

		this->bot_2.set_new_data(data_2);
	}
}

void Game1::update(float dt)
{
	this->gameover_timer += dt;
	this->new_game_timer += dt;

	if (this->gameover_timer >= 2.0f && this->new_game_timer >= 3.0f) {
		this->gameover_timer = 2.1f;
		this->new_game_timer = 3.1f;

		if (this->gameover_counter == 0) {
			this->board1.init();
			this->board2.init();
			this->new_game_timer = 0.0f;

			if (this->is_1_bot) {
				bot_data data_1;
				data_1.current = char_to_piece(this->board1.currentPiece);
				data_1.hold = PIECE_NONE;
				data_1.b2b = false;
				data_1.ren = 0;
				for (auto & p : this->board1.nextPiece) {
					data_1.next.add(char_to_piece(p));
				}
				for (int i = 0; i < 40; ++i) {
					data_1.board.row[i] = 0b0;
				}

				this->bot_1.set_new_data(data_1);

				this->input_timer_1 = 0.0f;
				this->solution_index_1 = 999;
				this->need_new_solution_1 = true;
				this->last_solution_1.clear();
				this->last_solution_1.shrink_to_fit();
			}

			if (this->is_2_bot) {
				bot_data data_2;
				data_2.current = char_to_piece(this->board2.currentPiece);
				data_2.hold = PIECE_NONE;
				data_2.b2b = false;
				data_2.ren = 0;
				for (auto & p : this->board2.nextPiece) {
					data_2.next.add(char_to_piece(p));
				}
				for (int i = 0; i < 40; ++i) {
					data_2.board.row[i] = 0b0;
				}

				this->bot_2.set_new_data(data_2);

				this->input_timer_2 = 0.0f;
				this->solution_index_2 = 999;
				this->need_new_solution_2 = true;
				this->last_solution_2.clear();
				this->last_solution_2.shrink_to_fit();
			}
		}

		++this->gameover_counter;
		if (this->gameover_counter > 1) {
			this->gameover_counter = 2;
		}

		if (this->is_1_bot && this->board1.lineClearDelayTimer >= 0.5f) {

			path_data solution;

			///*
			if (this->need_new_solution_1) {
				this->need_new_solution_1 = false;
				this->solution_index_1 = 0;
				solution = this->bot_1.request_solution();
				if (solution.is_hold) this->board1.hold();
				if (solution.is_soft_drop) {
					this->last_solution_1 = path_sd[solution.type][solution.index]; // out of range
				}
				else {
					this->last_solution_1 = path_hd[solution.type][solution.index]; // out of range
				}

				vec16<piece> next_q;
				next_q.size = 0;
				for (int i = 0; i < (int)this->board1.nextPiece.size(); ++i) {
					next_q.add(char_to_piece(this->board1.nextPiece[i]));
				}

				node old_n;
				node new_n;

				old_n.current = char_to_piece(this->board1.currentPiece);
				old_n.hold = char_to_piece(this->board1.holdPiece);
				old_n.b2b = this->board1.isB2bReady;
				old_n.ren = this->board1.ren;
				old_n.next = 0;
				for (int i = 0; i < 40; ++i) {
					uint16_t a_row = 0b0;
					for (int k = 0; k < 10; ++k) {
						if (this->board1.data[i][k] > 0) {
							a_row = a_row | (0b1 << (9 - k));
						}
					}
					old_n.board.row[i] = a_row;
				}

				old_n.attempt(new_n, false, this->last_solution_1, next_q);

				next_q.erase(0);

				bot_data new_data{ new_n.board, new_n.current, new_n.hold, next_q, new_n.b2b, new_n.ren };

				this->bot_1.set_new_data(new_data);

				this->board1.bot_node_count = solution.node_count;
				this->board1.bot_depth = solution.depth;
				//std::cout << "bot 1 " << solution.node_count << std::endl;
			}//*/

			if (this->solution_index_1 < (int)this->last_solution_1.size() && this->need_new_solution_1 == false) {
				this->input_timer_1 += dt;
				switch (this->last_solution_1[this->solution_index_1])
				{
				case MOVE_CW:
					this->board1.realCurrentPiece.tryRotate(1, this->board1.data);
					this->input_timer_1 = 0;
					++this->solution_index_1;
					break;
				case MOVE_CCW:
					this->board1.realCurrentPiece.tryRotate(3, this->board1.data);
					this->input_timer_1 = 0;
					++this->solution_index_1;
					break;
				case MOVE_LEFT:
					if (this->input_timer_1 >= this->bot_input_delay) {
						this->board1.realCurrentPiece.tryLeft(this->board1.data);
						++this->solution_index_1;
						this->input_timer_1 = 0;
					}
					break;
				case MOVE_RIGHT:
					if (this->input_timer_1 >= this->bot_input_delay) {
						this->board1.realCurrentPiece.tryRight(this->board1.data);
						++this->solution_index_1;
						this->input_timer_1 = 0;
					}
					break;
				case MOVE_DOWN:
					if (this->solution_index_1 == (int)this->last_solution_1.size() - 1) { // HD
						if (this->input_timer_1 >= this->bot_input_delay) {
							this->board1.realCurrentPiece.tryHardDrop(this->board1.data);
							this->solution_index_1 = 999;
							this->input_timer_1 = 0;
							this->need_new_solution_1 = true;
							this->last_solution_1 = std::vector<move>();
							this->board1.lineClearDelayTimer = 0.45f;
						}
					}
					else { // SD
						if (this->input_timer_1 >= this->bot_input_delay) {
							this->board1.realCurrentPiece.tryDown(this->board1.data);
							this->input_timer_1 = 0;
							if (this->board1.realCurrentPiece.isColliding(this->board1.realCurrentPiece.x, this->board1.realCurrentPiece.y + 1, this->board1.data)) {
								++this->solution_index_1;
							}
						}
					}
					break;
				default:
					break;
				}
			}
			else {
				//this->need_new_solution_1 = true;
				//this->solution_index_1 = 0;
				//this->last_solution_1 = " ";
			}

		}
		else {
			this->input_timer_1 = 0;
		}

		if (this->is_2_bot && this->board2.lineClearDelayTimer >= 0.5f) {

			path_data solution;

			///*
			if (this->need_new_solution_2) {
				this->need_new_solution_2 = false;
				this->solution_index_2 = 0;
				solution = this->bot_2.request_solution();
				if (solution.is_hold) this->board2.hold();
				if (solution.is_soft_drop) {
					this->last_solution_2 = path_sd[solution.type][solution.index]; // out of range
				}
				else {
					this->last_solution_2 = path_hd[solution.type][solution.index]; // out of range
				}

				vec16<piece> next_q;
				next_q.size = 0;
				for (int i = 0; i < (int)this->board2.nextPiece.size(); ++i) {
					next_q.add(char_to_piece(this->board2.nextPiece[i]));
				}

				node old_n;
				node new_n;

				old_n.current = char_to_piece(this->board2.currentPiece);
				old_n.hold = char_to_piece(this->board2.holdPiece);
				old_n.b2b = this->board2.isB2bReady;
				old_n.ren = this->board2.ren;
				old_n.next = 0;
				for (int i = 0; i < 40; ++i) {
					uint16_t a_row = 0b0;
					for (int k = 0; k < 10; ++k) {
						if (this->board2.data[i][k] > 0) {
							a_row = a_row | (0b1 << (9 - k));
						}
					}
					old_n.board.row[i] = a_row;
				}

				old_n.attempt(new_n, false, this->last_solution_2, next_q);

				next_q.erase(0);

				bot_data new_data{ new_n.board, new_n.current, new_n.hold, next_q, new_n.b2b, new_n.ren };

				this->bot_2.set_new_data(new_data);
				
				this->board2.bot_node_count = solution.node_count;
				this->board2.bot_depth = solution.depth;
				//std::cout << "bot 2 " << solution.node_count << std::endl;
			}//*/

			if (this->solution_index_2 < (int)this->last_solution_2.size() && this->need_new_solution_2 == false) {
				this->input_timer_2 += dt;
				switch (this->last_solution_2[this->solution_index_2])
				{
				case MOVE_CW:
					this->board2.realCurrentPiece.tryRotate(1, this->board2.data);
					this->input_timer_2 = 0;
					++this->solution_index_2;
					break;
				case MOVE_CCW:
					this->board2.realCurrentPiece.tryRotate(3, this->board2.data);
					this->input_timer_2 = 0;
					++this->solution_index_2;
					break;
				case MOVE_LEFT:
					if (this->input_timer_2 >= this->bot_input_delay) {
						this->board2.realCurrentPiece.tryLeft(this->board2.data);
						++this->solution_index_2;
						this->input_timer_2 = 0;
					}
					break;
				case MOVE_RIGHT:
					if (this->input_timer_2 >= this->bot_input_delay) {
						this->board2.realCurrentPiece.tryRight(this->board2.data);
						++this->solution_index_2;
						this->input_timer_2 = 0;
					}
					break;
				case MOVE_DOWN:
					if (this->solution_index_2 == (int)this->last_solution_2.size() - 1) {
						if (this->input_timer_2 >= this->bot_input_delay) {
							this->board2.realCurrentPiece.tryHardDrop(this->board2.data);
							this->solution_index_2 = 999;
							this->input_timer_2 = 0;
							this->need_new_solution_2 = true;
							this->last_solution_2 = std::vector<move>();
							this->board2.lineClearDelayTimer = 0.45f;
						}
					}
					else {
						if (this->input_timer_2 >= this->bot_input_delay) {
							this->board2.realCurrentPiece.tryDown(this->board2.data);
							this->input_timer_2 = 0;
							if (this->board2.realCurrentPiece.isColliding(this->board2.realCurrentPiece.x, this->board2.realCurrentPiece.y + 1, this->board2.data)) {
								++this->solution_index_2;
							}
						}
					}
					break;
				default:
					break;
				}
			}
			else {
				//this->need_new_solution_1 = true;
				//this->solution_index_1 = 0;
				//this->last_solution_1 = " ";
			}

		}
		else {
			this->input_timer_2 = 0;
		}

		this->board1.update(dt);
		this->board2.update(dt);

		if (this->board1.isGameover() || this->board2.isGameover()) {
			this->gameover_timer = 0.0f;
			this->gameover_counter = 0;
			std::cout << "BOARD 1 - TOTAL SENT: " << this->board1.total_line_sent << std::endl;
			std::cout << "BOARD 2 - TOTAL SENT: " << this->board2.total_line_sent << std::endl;
		}

	}

};

void Game1::render()
{
	this->canvas.clear();
	this->board1.render(&this->canvas);
	this->board2.render(&this->canvas);

	if (this->new_game_timer < 3.0f) { // i don't want to do math :<
		this->sprite_image_of_number.setOrigin(10, 15);
		this->sprite_image_of_number.setPosition(960, 525);
		this->sprite_image_of_number.setScale(4, 4);
		if (this->new_game_timer < 1.0f) {
			this->sprite_image_of_number.setTextureRect(sf::IntRect(40, 0, 20, 30));
		}
		else if (this->new_game_timer < 2.0f) {
			this->sprite_image_of_number.setTextureRect(sf::IntRect(20, 0, 20, 30));
		}
		else {
			this->sprite_image_of_number.setTextureRect(sf::IntRect(0, 0, 20, 30));
		}
		this->canvas.draw(this->sprite_image_of_number);
	}
};

void Game1::unload()
{
	if (this->is_1_bot) this->bot_1.thread_destroy();
	if (this->is_2_bot) this->bot_2.thread_destroy();
};
