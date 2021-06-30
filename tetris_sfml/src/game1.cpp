#include "game1.h"

void game1::load()
{
	srand(time(NULL));
	this->img_counter.loadFromFile("res/graphic/counter.png");
	this->sp_counter.setTexture(this->img_counter);
	this->board_1.set_enemy(&this->board_2);
	this->board_2.set_enemy(&this->board_1);
	this->board_1.init();
	this->board_2.init();
	this->gameover_counter = 119;
}

void game1::update(double dt)
{
	this->gameover_counter++;

	if (this->gameover_counter == 120)
		this->start_game();

	if (this->gameover_counter >= 300) {
		this->gameover_counter = 301;
		this->update_game(dt);
		if (this->board_1.is_game_over() || this->board_2.is_game_over()) {
			this->gameover_counter = 0;
			this->end_game();
		}
	}
}

void game1::render()
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

void game1::unload()
{
}

void game1::start_game()
{
	this->board_1.init();
	this->board_2.init();
}

void game1::update_game(double dt)
{
	this->board_1.update(dt);
	this->board_2.update(dt);
}

void game1::end_game()
{
}
