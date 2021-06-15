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

	/*
	std::string answer1;

	std::cout << "	- 0 - INIT RANDOM" << std::endl;
	std::cout << "	- 1 - LOAD FROM PREVIOUS" << std::endl;
	std::cin >> answer1;

	if (answer1 == "0") {
		game.opt.init();
	}
	else if (answer1 == "1") {
		std::string l_dir;
		std::cout << "LOAD DIRECTORY (JSON): " << std::endl;
		std::cin >> l_dir;
		game.opt.load(l_dir);
	}

	system("CLS");

	std::cout << "SAVE DIRECTORY: " << std::endl;
	std::cin >> game.save_dir;

	system("CLS");
	*/

	game.is_1_bot = true;
	game.is_2_bot = true;
	game.start();

	//std::cin.get();
	return 0;
}