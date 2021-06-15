#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <algorithm>
#include <math.h>
#include "Piece.h"

namespace Tetris {
	class Board
	{
	public:
		Board();

		int x, y;

		std::array<std::array<int, 10>, 40> data;
		char holdPiece = ' ';
		char currentPiece = ' ';
		std::vector<char> nextPiece;
		std::array<char, 7> bag = { 'I', 'J', 'L', 'S', 'T', 'Z', 'O' };
		sf::Color color[8] = { sf::Color(0, 255, 255, 255), sf::Color(0, 0, 255, 255), sf::Color(255, 165, 0, 255), sf::Color(255, 255, 0, 255), sf::Color(124, 252, 0, 255), sf::Color(153, 0, 255, 255), sf::Color(255, 0, 0, 255), sf::Color(192, 192, 192, 255) };

		void init();
		void setEnemy(Board *_enemy);
		void setPosScale(int _x, int _y, float _scale);
		void shuffleBag();
		void fillNextPiece();
		void changePiece();

		bool isGameover();
		void placePiece();
		void hold();

		std::vector<int> fullRowList();
		void clearFullRow(std::vector<int> full_row_list);
		int countGarbageSent(bool t_spin, std::vector<int> full_row_list);
		void sendGarbage(int garbage);
		void placeGarbage();

		void update(float dt);
		void renderData();
		void render(sf::RenderTexture* _canvas);

		static std::string encodeData(std::array<std::array<int, 10>, 40> board_data);
		static std::array<std::array <int, 10>, 40> decodeData(std::string code);

		static const int numberOfPreview = 12;

		void enableAI();

		Piece realCurrentPiece;

		bool isB2bReady = false;
		int ren = 0;

		float lineClearDelayTimer = 0;

		int total_line_sent = 0;
		int total_t_clear = 0;
		int total_tetris = 0;

		sf::SoundBuffer sound_clear;
		sf::Sound sound;
	private:
		Board *enemy = nullptr;

		sf::RenderTexture texture;
		sf::VertexArray quads;
		sf::VertexArray decor;
		sf::Sprite sprite;

		int garbage = 0;

		float margin_time = 0.0f;

		bool is_ai_enable = false;

		int numberOfPieceTakenFromBag = 0;
		int holdCounter = 0;
		float inGameTimer = 0;

		std::array<int, 8> g_per_dist = { 1, 1, 2, 2, 3, 4, 4, 4 };

		sf::Texture image_of_pieces;
		sf::Sprite sprite_image_of_pieces;
		std::unordered_map<char, int> pos_of_piece_in_image;
	};

};
