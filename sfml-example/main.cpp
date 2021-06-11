#include <iostream>
#include <stdlib.h>
#include <string>
#include <bitset>
#include <chrono>

#include "Game1.h"
#include "Game2.h"

int main()
{
	system("Title TETRIS BOT");
	srand(time(NULL));

	Game1 game;
	game.is_1_bot = true;
	game.is_2_bot = true;
	game.start();

	//std::cin.get();
	return 0;
}