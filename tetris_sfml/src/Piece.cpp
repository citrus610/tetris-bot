#include "Piece.h"

Tetris::Piece::Piece()
{

	//Set piece init position
	this->initPos['I'] = 3;
	this->initPos['J'] = 3;
	this->initPos['L'] = 3;
	this->initPos['S'] = 3;
	this->initPos['T'] = 3;
	this->initPos['Z'] = 3;
	this->initPos['O'] = 4;

	//Set piece forms
	this->form['I'] = {
	{
	  {0, 0, 0, 0},
	  {1, 1, 1, 1},
	  {0, 0, 0, 0},
	  {0, 0, 0, 0}
	},
	{
	  {0, 0, 1, 0},
	  {0, 0, 1, 0},
	  {0, 0, 1, 0},
	  {0, 0, 1, 0}
	},
	{
	  {0, 0, 0, 0},
	  {0, 0, 0, 0},
	  {1, 1, 1, 1},
	  {0, 0, 0, 0}
	},
	{
	  {0, 1, 0, 0},
	  {0, 1, 0, 0},
	  {0, 1, 0, 0},
	  {0, 1, 0, 0}
	}
	};
	this->form['J'] = {
	{
	  {2, 0, 0},
	  {2, 2, 2},
	  {0, 0, 0}
	},
	{
	  {0, 2, 2},
	  {0, 2, 0},
	  {0, 2, 0}
	},
	{
	  {0, 0, 0},
	  {2, 2, 2},
	  {0, 0, 2}
	},
	{
	  {0, 2, 0},
	  {0, 2, 0},
	  {2, 2, 0}
	}
	};
	this->form['L'] = {
	{
	  {0, 0, 3},
	  {3, 3, 3},
	  {0, 0, 0}
	},
	{
	  {0, 3, 0},
	  {0, 3, 0},
	  {0, 3, 3}
	},
	{
	  {0, 0, 0},
	  {3, 3, 3},
	  {3, 0, 0}
	},
	{
	  {3, 3, 0},
	  {0, 3, 0},
	  {0, 3, 0}
	}
	};
	this->form['S'] = {
	{
	  {0, 5, 5},
	  {5, 5, 0},
	  {0, 0, 0}
	},
	{
	  {0, 5, 0},
	  {0, 5, 5},
	  {0, 0, 5}
	},
	{
	  {0, 0, 0},
	  {0, 5, 5},
	  {5, 5, 0}
	},
	{
	  {5, 0, 0},
	  {5, 5, 0},
	  {0, 5, 0}
	}
	};
	this->form['T'] = {
	{
	  {0, 6, 0},
	  {6, 6, 6},
	  {0, 0, 0}
	},
	{
	  {0, 6, 0},
	  {0, 6, 6},
	  {0, 6, 0}
	},
	{
	  {0, 0, 0},
	  {6, 6, 6},
	  {0, 6, 0}
	},
	{
	  {0, 6, 0},
	  {6, 6, 0},
	  {0, 6, 0}
	}
	};
	this->form['Z'] = {
	{
	  {7, 7, 0},
	  {0, 7, 7},
	  {0, 0, 0}
	},
	{
	  {0, 0, 7},
	  {0, 7, 7},
	  {0, 7, 0}
	},
	{
	  {0, 0, 0},
	  {7, 7, 0},
	  {0, 7, 7}
	},
	{
	  {0, 7, 0},
	  {7, 7, 0},
	  {7, 0, 0}
	}
	};
	this->form['O'] = {
	{
	  {4, 4},
	  {4, 4}
	}
	};

	//Set piece srs data
	this->srsData['I'] = {
	{{ -2, 0 }, { 1, 0 }, { -2, 1 }, { 1, -2 }},
	{{ 2, 0 }, { -1, 0 }, { 2, -1 }, { -1, 2 }},
	{{ -1, 0 }, { 2, 0 }, { -1, -2 }, { 2, 1 }},
	{{ 1, 0 }, { -2, 0 }, { 1, 2 }, { -2, -1 }}
	};
	this->srsData['J'] = {
	{{ -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 }},
	{{ 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 }},
	{{ 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 }},
	{{ -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 }}
	};
	this->srsData['L'] = this->srsData['J'];
	this->srsData['S'] = this->srsData['J'];
	this->srsData['T'] = this->srsData['J'];
	this->srsData['Z'] = this->srsData['J'];
	this->srsData['O'] = { { {} } };

	this->sound_place.loadFromFile("Content/Audio/place.wav");
	this->sound_move.loadFromFile("Content/Audio/move.wav");
	//this->sound.setVolume(5.0f);
	this->sound.setVolume(0.0f);
}

void Tetris::Piece::setType(char type)
{
	this->type = type;
}

char Tetris::Piece::getType()
{
	return this->type;
}

bool Tetris::Piece::isLocked()
{
	return this->locked;
}

bool Tetris::Piece::isColliding(int _x, int _y, std::array<std::array<int, 10>, 40> board_data)
{
	for (int __y = 0; __y < (int)this->form[this->getType()][this->currentForm].size(); __y++) {
		for (int __x = 0; __x < (int)this->form[this->getType()][this->currentForm][__y].size(); __x++) {
			if (__y + _y < 40 && (__x + _x >= 0 || __x + _x < 10)) {

			}
			if (this->form[this->getType()][this->currentForm][__y][__x] > 0 && __y + _y > 39) {
				return true;
			}
			if (this->form[this->getType()][this->currentForm][__y][__x] > 0 && (__x + _x < 0 || __x + _x > 9)) {
				return true;
			}
			if (this->form[this->getType()][this->currentForm][__y][__x] > 0 && board_data[_y + __y][_x + __x] > 0) {
				return true;
			}
		}
	}
	return false;
}

bool Tetris::Piece::isImmobile(std::array<std::array<int, 10>, 40> board_data)
{
	return this->isColliding(this->x - 1, this->y, board_data) && this->isColliding(this->x + 1, this->y, board_data) && this->isColliding(this->x, this->y - 1, board_data);
}

void Tetris::Piece::init()
{
	this->x = this->initPos[this->getType()];
	this->y = 18;
	this->locked = false;
	this->currentForm = 0;
}

void Tetris::Piece::lock()
{
	this->sound.setBuffer(this->sound_place);
	this->sound.play();
	this->locked = true;
}

void Tetris::Piece::tryLeft(std::array<std::array<int, 10>, 40> board_data)
{
	if (!this->isColliding(this->x - 1, this->y, board_data)) {
		this->sound.setBuffer(this->sound_move);
		this->sound.play();
		this->x--;
		this->onFloorTimer = 0;
	}
}

void Tetris::Piece::tryRight(std::array<std::array<int, 10>, 40> board_data)
{
	if (!this->isColliding(this->x + 1, this->y, board_data)) {
		this->sound.setBuffer(this->sound_move);
		this->sound.play();
		this->x++;
		this->onFloorTimer = 0;
	}
}

void Tetris::Piece::tryDown(std::array<std::array<int, 10>, 40> board_data)
{
	if (!this->isColliding(this->x, this->y + 1, board_data)) {
		this->y++;
	}
}

void Tetris::Piece::tryHardDrop(std::array<std::array<int, 10>, 40> board_data)
{
	while (!this->isColliding(this->x, this->y + 1, board_data)) {
		this->y++;
	}
	this->lock();
}

void Tetris::Piece::tryRotate(int direction, std::array<std::array<int, 10>, 40> board_data)
{
	/*
	direction = 1 -> rotate right
	direction = 3 -> rotate left
	*/
	int oldForm = this->currentForm;
	this->currentForm = (this->currentForm + direction) % (this->form[this->getType()].size());
	int k = 0;
	switch (this->getType())
	{
	case 'I':
		if ((oldForm == 0 && this->currentForm == 1) || (oldForm == 3 && this->currentForm == 2)) //0 > R && L > 2
			k = 0;
		else if ((oldForm == 1 && this->currentForm == 0) || (oldForm == 2 && this->currentForm == 3)) //R > 0 && 2 > L
			k = 1;
		else if ((oldForm == 1 && this->currentForm == 2) || (oldForm == 0 && this->currentForm == 3)) //R > 2 && 0 > L
			k = 2;
		else if ((oldForm == 2 && this->currentForm == 1) || (oldForm == 3 && this->currentForm == 0)) //2 > R && L > 0
			k = 3;
		break;
	case 'O':
		break;
	default:
		if (this->currentForm == 1) //From smt to R
			k = 0;
		else if (this->currentForm == 3) //From smt to L
			k = 2;
		else
			if (oldForm == 1) //From R to smt
				k = 1;
			else if (oldForm == 3) //From R to smt
				k = 3;
		break;
	}
	if (this->getType() != 'O') {
		if (this->isColliding(this->x, this->y, board_data)) {
			int c = 0;
			for (int i = 0; i < 4; i++) {
				if (!this->isColliding(this->x + this->srsData[this->getType()][k][i][0], this->y + this->srsData[this->getType()][k][i][1], board_data)) {
					this->x += this->srsData[this->getType()][k][i][0];
					this->y += this->srsData[this->getType()][k][i][1];
					break;
				}
				c++;
			}
			if (c == 4) {
				this->currentForm = oldForm;
			}
		}
	}
	this->onFloorTimer = 0;
}

void Tetris::Piece::checkForLock(float dt, std::array<std::array<int, 10>, 40> board_data)
{
	if (this->isColliding(this->x, this->y + 1, board_data)) {
		this->onFloorTimer += dt;
	}
	else {
		this->onFloorTimer = 0;
	}
	if (this->onFloorTimer >= 0.5f) {
		this->lock();
	}
}

bool Tetris::Piece::justTSpin(std::array<std::array<int, 10>, 40> board_data)
{
	return this->getType() == 'T' && this->isImmobile(board_data);
}

void Tetris::Piece::update(float dt, std::array<std::array<int, 10>, 40> board_data)
{
	if (!this->aiEnable) {
		// Das
		if (Input::right.isPressed()) {
			if (this->dasDelayTimer == 0) { this->tryRight(board_data); }
			this->dasDelayTimer += dt;
			if (this->dasDelayTimer >= 0.25f) {
				this->dasDelayTimer = 0.25f;
				if (this->dasAutoRepeatTimer == 0) { this->tryRight(board_data); }
				this->dasAutoRepeatTimer += dt;
				if (this->dasAutoRepeatTimer >= 0.03f) {
					this->dasAutoRepeatTimer = 0;
				}
			}
		}
		else if (Input::left.isPressed()) {
			if (this->dasDelayTimer == 0) { this->tryLeft(board_data); }
			this->dasDelayTimer += dt;
			if (this->dasDelayTimer >= 0.25f) {
				this->dasDelayTimer = 0.25f;
				if (this->dasAutoRepeatTimer == 0) { this->tryLeft(board_data); }
				this->dasAutoRepeatTimer += dt;
				if (this->dasAutoRepeatTimer >= 0.03f) {
					this->dasAutoRepeatTimer = 0;
				}
			}
		}
		else {
			this->dasAutoRepeatTimer = 0;
			this->dasDelayTimer = 0;
		}

		// Hard drop
		if (Input::space.justPressed()) {
			this->tryHardDrop(board_data);
		}

		// Soft drop
		if (Input::down.isPressed()) {
			if (Input::down.justPressed()) {
				this->tryDown(board_data);
			}
			this->softDropTimer += dt;
			if (this->softDropTimer >= 0.03f) {
				this->softDropTimer = 0;
				this->tryDown(board_data);
			}
		}
		else { this->softDropTimer = 0; }

		// Rotating
		if (Input::up.justPressed()) {
			this->tryRotate(1, board_data);
		}
		if (Input::z.justPressed()) {
			this->tryRotate(3, board_data);
		}
	}

	this->checkForLock(dt, board_data);
}