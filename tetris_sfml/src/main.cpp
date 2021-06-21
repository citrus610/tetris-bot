#include <iostream>
#include <stdlib.h>
#include <string>
#include <bitset>
#include <chrono>

#include "Game1.h"

int main()
{
	system("Title TETRIS BOT");
	srand(time(NULL));

	create_init_js();

	Game1 game;
	game.is_1_bot = true;
	game.is_2_bot = true;
	game.start();

	//std::cin.get();
	return 0;
}