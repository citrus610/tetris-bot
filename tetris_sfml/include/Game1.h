#ifndef SFML_GAME1_H
#define SFML_GAME1_H

#include "Game.h"
#include "Type.h"

class Game1 :
	public Game
{
public:
	void load() override;
	void update(float dt) override;
	void render() override;
	void unload() override;
public:
	Tetris::Board board1, board2;

	float gameover_timer = 0.0f;
	float new_game_timer = 0;
	int gameover_counter = 0;

	sf::Texture image_of_number;
	sf::Sprite sprite_image_of_number;

	bool is_1_bot = false;
	bool is_2_bot = false;

	float bot_input_delay = 0.04666f;

	bot bot_1;
	weight w_1;
	std::vector<move> last_solution_1;
	bool need_new_solution_1 = true;
	int solution_index_1 = 999;
	float input_timer_1 = 0.0f;

	bot bot_2;
	weight w_2;
	std::vector<move> last_solution_2;
	bool need_new_solution_2 = true;
	int solution_index_2 = 999;
	float input_timer_2 = 0.0f;
};

#endif