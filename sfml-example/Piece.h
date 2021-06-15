#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <array>

#include <SFML/Audio.hpp>

#include "Input.h"

namespace Tetris {
	class Piece
	{
	public:
		Piece();

		int x = 0;
		int y = 0;
		int currentForm = 0;

		void setType(char type);
		char getType();

		bool isLocked();
		bool isColliding(int _x, int _y, std::array<std::array<int, 10>, 40> board_data);
		bool isImmobile(std::array<std::array<int, 10>, 40> board_data);

		void init();
		void lock();

		void tryLeft(std::array<std::array<int, 10>, 40> board_data);
		void tryRight(std::array<std::array<int, 10>, 40> board_data);
		void tryDown(std::array<std::array<int, 10>, 40> board_data);
		void tryHardDrop(std::array<std::array<int, 10>, 40> board_data);
		void tryRotate(int direction, std::array<std::array<int, 10>, 40> board_data);

		void checkForLock(float dt, std::array<std::array<int, 10>, 40> board_data);

		bool justTSpin(std::array<std::array<int, 10>, 40> board_data);

		void update(float dt, std::array<std::array<int, 10>, 40> board_data);

		std::unordered_map<char, std::vector<std::vector<std::vector<int>>>> form;

		bool aiEnable = false;

		sf::SoundBuffer sound_move, sound_place, sound_clear;
		sf::Sound sound;
	private:
		char type = ' ';
		bool locked = false;
		std::unordered_map<char, int> initPos;
		std::unordered_map<char, std::vector<std::vector<std::vector<int>>>> srsData;

		float onFloorTimer = 0;
		float dasDelayTimer = 0;
		float dasAutoRepeatTimer = 0;
		float softDropTimer = 0;
	};
}