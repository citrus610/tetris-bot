#include "Board.h"
#include <stdlib.h>

Tetris::Board::Board()
{
	this->sound_clear.loadFromFile("Content/Audio/clear.wav");
	this->sound.setVolume(20.f);
}

void Tetris::Board::init()
{
	for (int i = 0; i < 40; i++) {
		for (int k = 0; k < 10; k++) {
			this->data[i][k] = 0;
		}
	}

	this->numberOfPieceTakenFromBag = 0;
	this->nextPiece.clear();
	this->holdPiece = ' ';
	this->currentPiece = ' ';
	this->garbage = 0;
	this->isB2bReady = false;
	this->ren = 0;

	this->shuffleBag();
	this->fillNextPiece();
	this->changePiece();

	// graphic stuffs
	texture.create(20, 20);
	this->quads = sf::VertexArray(sf::Quads);
	// 0, 0, 0, 0
	// 255, 255, 255, 255
	for (int i = 0; i < 200; i++) {

		this->quads.append(sf::Vertex(sf::Vector2f((i % 10 + 5) * 1, (i - (i % 10)) / 10 * 1), this->color[0]));
		this->quads.append(sf::Vertex(sf::Vector2f((i % 10 + 5) * 1 + 1, (i - (i % 10)) / 10 * 1), this->color[0]));
		this->quads.append(sf::Vertex(sf::Vector2f((i % 10 + 5) * 1 + 1, (i - (i % 10)) / 10 * 1 + 1), this->color[0]));
		this->quads.append(sf::Vertex(sf::Vector2f((i % 10 + 5) * 1, (i - (i % 10)) / 10 * 1 + 1), this->color[0]));
	}

	this->decor = sf::VertexArray(sf::Quads);

	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red)); // draw red garbage warning line
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));
	this->decor.append(sf::Vertex(sf::Vector2f(0, 0), sf::Color::Red));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x - 8, this->y - 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20 + 8, this->y - 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20 + 8, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x - 8, this->y), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x - 8, this->y - 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x, this->y - 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x, this->y + this->sprite.getScale().x * 20 + 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x - 8, this->y + this->sprite.getScale().x * 20 + 8), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20, this->y - 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20 + 8, this->y - 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20 + 8, this->y + this->sprite.getScale().x * 20 + 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20, this->y + this->sprite.getScale().x * 20 + 8), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x - 8, this->y + this->sprite.getScale().x * 20), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20 + 8, this->y + this->sprite.getScale().x * 20), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 20 + 8, this->y + this->sprite.getScale().x * 20 + 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x - 8, this->y + this->sprite.getScale().x * 20 + 8), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 4, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 4 + 8, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 4 + 8, this->y + this->sprite.getScale().x * 20), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 4, this->y + this->sprite.getScale().x * 20), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 5, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 5 - 8, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 5 - 8, this->y + this->sprite.getScale().x * 20), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 5, this->y + this->sprite.getScale().x * 20), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 15, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 15 + 8, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 15 + 8, this->y + this->sprite.getScale().x * 20), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 15, this->y + this->sprite.getScale().x * 20), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 16, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 16 - 8, this->y), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 16 - 8, this->y + this->sprite.getScale().x * 20), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 16, this->y + this->sprite.getScale().x * 20), sf::Color::White));

	this->decor.append(sf::Vertex(sf::Vector2f(this->x, this->y + this->sprite.getScale().x * 4), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 4, this->y + this->sprite.getScale().x * 4), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * 4, this->y + this->sprite.getScale().x * 4 + 8), sf::Color::White));
	this->decor.append(sf::Vertex(sf::Vector2f(this->x, this->y + this->sprite.getScale().x * 4 + 8), sf::Color::White));
	for (int i = 0; i < 19; i++) { // draw caro dot
		for (int k = 0; k < 9; k++) {
			this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * (6 + k) - 2, this->y + this->sprite.getScale().x * (1 + i) - 2), sf::Color(32, 32, 32, 255)));
			this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * (6 + k) + 2, this->y + this->sprite.getScale().x * (1 + i) - 2), sf::Color(32, 32, 32, 255)));
			this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * (6 + k) + 2, this->y + this->sprite.getScale().x * (1 + i) + 2), sf::Color(32, 32, 32, 255)));
			this->decor.append(sf::Vertex(sf::Vector2f(this->x + this->sprite.getScale().x * (6 + k) - 2, this->y + this->sprite.getScale().x * (1 + i) + 2), sf::Color(32, 32, 32, 255)));
		}
	}

	this->image_of_pieces.loadFromFile("Content/Graphic/Tetrisminos.png");
	this->image_of_pieces.setSmooth(false);
	this->sprite_image_of_pieces.setTexture(Tetris::Board::image_of_pieces);
	this->pos_of_piece_in_image['I'] = 0;
	this->pos_of_piece_in_image['J'] = 1;
	this->pos_of_piece_in_image['L'] = 2;
	this->pos_of_piece_in_image['S'] = 3;
	this->pos_of_piece_in_image['T'] = 4;
	this->pos_of_piece_in_image['Z'] = 5;
	this->pos_of_piece_in_image['O'] = 6;

	this->total_line_sent = 0;
	this->total_t_clear = 0;
	this->total_tetris = 0;
}

void Tetris::Board::setEnemy(Board* _enemy)
{
	this->enemy = _enemy;
}

void Tetris::Board::setPosScale(int _x, int _y, float _scale)
{
	this->x = _x;
	this->y = _y;
	this->sprite.setScale(sf::Vector2f(_scale, _scale));
	this->sprite.setPosition(this->x, this->y);
}

void Tetris::Board::shuffleBag()
{
	int n = 7;
	while (n > 1)
	{
		n--;
		int k = rand() % (n + 1);
		char value = this->bag[k];
		this->bag[k] = this->bag[n];
		this->bag[n] = value;
	}
}

void Tetris::Board::fillNextPiece()
{
	int nextpiececounter = this->nextPiece.size();
	for (int i = 0; i < Tetris::Board::numberOfPreview - nextpiececounter; i++) {
		this->nextPiece.push_back(this->bag[this->numberOfPieceTakenFromBag]);
		this->numberOfPieceTakenFromBag++;
		if (this->numberOfPieceTakenFromBag >= 7) {
			this->numberOfPieceTakenFromBag = 0;
			this->shuffleBag();
		}
	}
}

void Tetris::Board::changePiece()
{
	this->currentPiece = this->nextPiece[0];
	this->realCurrentPiece.setType(this->currentPiece);
	this->realCurrentPiece.init();
	this->nextPiece.erase(this->nextPiece.begin() + 0);
	this->fillNextPiece();
}

bool Tetris::Board::isGameover()
{
	return this->realCurrentPiece.isLocked() && this->realCurrentPiece.y <= 18 && (this->realCurrentPiece.x == 3 || this->realCurrentPiece.x == 4);
}

void Tetris::Board::placePiece()
{
	for (int _y = 0; _y < (int)this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm].size(); _y++) {
		for (int _x = 0; _x < (int)this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm][_y].size(); _x++) {
			if (this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm][_y][_x] > 0) {
				this->data[_y + this->realCurrentPiece.y][_x + this->realCurrentPiece.x] = this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm][_y][_x];
			}
		}
	}
}

void Tetris::Board::hold()
{
	if (holdCounter == 0) {
		holdCounter = 1;
		if (this->holdPiece == ' ') {
			this->holdPiece = this->currentPiece;
			this->changePiece();
		}
		else {
			char place_holder = this->holdPiece;
			this->holdPiece = this->currentPiece;
			this->currentPiece = place_holder;
			this->realCurrentPiece.setType(this->currentPiece);
			this->realCurrentPiece.init();
		}
	}
}

std::vector<int> Tetris::Board::fullRowList()
{
	std::vector<int> result;
	for (int i = 20; i < 40; i++) {
		int row_product = 1;
		for (int k = 0; k < 10; k++) {
			row_product = row_product * this->data[i][k];
		}
		if (row_product > 0) {
			result.push_back(i);
		}
	}
	return result;
}

void Tetris::Board::clearFullRow(std::vector<int> full_row_list)
{
	if (!full_row_list.empty()) {
		//this->sound.setBuffer(this->sound_clear);
		//this->sound.play();
	}
	for (int i = 0; i < (int)full_row_list.size(); i++) {
		for (int k = full_row_list[i]; k > 0; k--) {
			this->data[k] = this->data[k - 1];
		}
		this->data[0] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	}
}

int Tetris::Board::countGarbageSent(bool t_spin, std::vector<int> full_row_list)
{
	int result = 0;

	if (t_spin) {
		result = full_row_list.size() * 2;
		if (this->isB2bReady)
			result++;
		this->isB2bReady = true;
	}
	else {
		switch (full_row_list.size())
		{
		case 0:
			break;
		case 4:
			result = 4;
			if (this->isB2bReady)
				result++;
			this->isB2bReady = true;
			break;
		default:
			this->isB2bReady = false;
			result = full_row_list.size() - 1;
			break;
		}
	}

	// find if perfect clear
	int not_full_or_full_row_count = 0;
	for (int i = 20; i < 40; i++) {
		int row_sum = 0;
		int row_product = 1;
		for (int k = 0; k < 10; k++) {
			row_sum += this->data[i][k];
			row_product *= this->data[i][k];
		}
		if ((row_sum == 0) || (row_product > 0)) {
			not_full_or_full_row_count++;
		}
	}
	if (not_full_or_full_row_count == 20) {
		result = 10;
	}

	// ren
	if (full_row_list.size() > 0) {
		this->ren++;
	}
	else {
		this->ren = 0;
	}
	if (this->ren < 10) {
		result += (this->ren - (this->ren % 2)) / 2;
	}
	else if (this->ren == 10) {
		result += 4;
	}
	else {
		result += 5;
	}

	return result;
}

void Tetris::Board::sendGarbage(int garbage)
{
	if (this->enemy->garbage == 0) {
		this->enemy->margin_time = 0.0f;
	}
	this->enemy->garbage += garbage;
}

void Tetris::Board::placeGarbage()
{
	if (this->garbage > 39) {
		this->garbage = 22;
	}

	std::vector<int> g_dist;

	if (this->garbage < 1) {
		return;
	}

	if (this->garbage < 4) {
		g_dist.push_back(this->garbage);
	}
	else {
		int g_remain = 0;
		while (g_remain < this->garbage)
		{
			int to_add = std::min(this->garbage - g_remain, this->g_per_dist[rand() % 8]);
			g_remain += to_add;
			g_dist.push_back(to_add);
		}
	}

	// shift the row up
	for (int i = 0; i < 40 - this->garbage; i++) {
		this->data[i] = this->data[i + this->garbage];
	}

	std::vector<std::array<int, 10>> g_line_to_add;
	for (int i = 0; i < (int)g_dist.size(); i++) {
		int hole_pos = rand() % 8;
		for (int k = 0; k < g_dist[i]; k++) {
			std::array<int, 10> to_add = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 };
			to_add[hole_pos] = 0;
			g_line_to_add.push_back(to_add);
		}
	}

	for (int i = 0; i < this->garbage; i++) {
		this->data[39 - i] = g_line_to_add[i];
	}

	this->garbage = 0;
}

void Tetris::Board::update(float dt)
{
	this->lineClearDelayTimer += dt;
	this->lineClearDelayTimer = std::min(0.6f, this->lineClearDelayTimer);
	if (this->lineClearDelayTimer >= 0.5f) {
		this->margin_time += dt;
		this->clearFullRow(this->fullRowList());
		this->realCurrentPiece.update(dt, this->data);
		if (this->realCurrentPiece.isLocked())
			this->holdCounter = 0;
		if (!this->isGameover()) {
			this->inGameTimer += dt;
			if (this->inGameTimer >= 0.5f) {
				this->inGameTimer = 0;
				if (!Input::down.isPressed() && !this->is_ai_enable) {
					this->realCurrentPiece.tryDown(this->data);
				}
			}
			if (Input::c.justPressed() && !this->is_ai_enable)
				this->hold();
			if (this->realCurrentPiece.isLocked()) {
				bool t_spin = this->realCurrentPiece.justTSpin(this->data);
				this->placePiece();
				std::vector<int> full_row_list = this->fullRowList();
				int count_garbage = this->countGarbageSent(t_spin, full_row_list);
				this->total_line_sent += count_garbage;
				if (full_row_list.size() > 0 && t_spin) this->total_t_clear += (int)full_row_list.size();
				if (full_row_list.size() >= 4) this->total_tetris += 1;
				if (full_row_list.size() < 1) {
					if (this->margin_time >= 0.33f) {
						this->placeGarbage();
					}
				}
				else {
					if (this->garbage > count_garbage) {
						this->garbage -= count_garbage;
						if (this->margin_time >= 0.33f) {
							this->placeGarbage();
						}
					}
					else {
						this->sendGarbage(count_garbage - this->garbage);
						this->garbage = 0;
					}
					this->lineClearDelayTimer = 0;
					this->sound.setBuffer(this->sound_clear);
					this->sound.play();
				}
				this->changePiece();
			}
		}
	}
}

void Tetris::Board::renderData()
{
	this->texture.clear();


	// draw data
	for (int i = 20; i < 40; i++) {
		for (int k = 0; k < 10; k++) {
			if (this->data[i][k] > 0) {
				this->quads[((i - 20) * 10 + k) * 4].color = this->color[this->data[i][k] - 1];
				this->quads[((i - 20) * 10 + k) * 4 + 1].color = this->color[this->data[i][k] - 1];
				this->quads[((i - 20) * 10 + k) * 4 + 2].color = this->color[this->data[i][k] - 1];
				this->quads[((i - 20) * 10 + k) * 4 + 3].color = this->color[this->data[i][k] - 1];
			}
			else {
				this->quads[((i - 20) * 10 + k) * 4].color = sf::Color(0, 0, 0, 0);
				this->quads[((i - 20) * 10 + k) * 4 + 1].color = sf::Color(0, 0, 0, 0);
				this->quads[((i - 20) * 10 + k) * 4 + 2].color = sf::Color(0, 0, 0, 0);
				this->quads[((i - 20) * 10 + k) * 4 + 3].color = sf::Color(0, 0, 0, 0);
			}
		}
	}

	// draw current piece and ghost piece
	int _y = this->realCurrentPiece.y; // find ghost piece y position
	while (!this->realCurrentPiece.isColliding(this->realCurrentPiece.x, _y + 1, this->data)) { _y++; }
	for (int i = 0; i < (int)this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm].size(); i++) {
		for (int k = 0; k < (int)this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm][i].size(); k++) {
			if (this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm][i][k] > 0) {
				//draw ghost piece
				if (i + _y >= 20) {
					this->quads[((i + _y - 20) * 10 + k + this->realCurrentPiece.x) * 4].color = sf::Color(255, 255, 255, 64);
					this->quads[((i + _y - 20) * 10 + k + this->realCurrentPiece.x) * 4 + 1].color = sf::Color(255, 255, 255, 64);
					this->quads[((i + _y - 20) * 10 + k + this->realCurrentPiece.x) * 4 + 2].color = sf::Color(255, 255, 255, 64);
					this->quads[((i + _y - 20) * 10 + k + this->realCurrentPiece.x) * 4 + 3].color = sf::Color(255, 255, 255, 64);
				}
				// draw current piece
				if (i + this->realCurrentPiece.y >= 20) {
					int value = this->realCurrentPiece.form[this->currentPiece][this->realCurrentPiece.currentForm][i][k];
					this->quads[((i + this->realCurrentPiece.y - 20) * 10 + k + this->realCurrentPiece.x) * 4].color = this->color[value - 1];
					this->quads[((i + this->realCurrentPiece.y - 20) * 10 + k + this->realCurrentPiece.x) * 4 + 1].color = this->color[value - 1];
					this->quads[((i + this->realCurrentPiece.y - 20) * 10 + k + this->realCurrentPiece.x) * 4 + 2].color = this->color[value - 1];
					this->quads[((i + this->realCurrentPiece.y - 20) * 10 + k + this->realCurrentPiece.x) * 4 + 3].color = this->color[value - 1];
				}
			}
		}
	}

	// draw clear lines
	std::vector<int> full_line = this->fullRowList();
	if (this->lineClearDelayTimer < 0.3f) {
		for (auto & i : full_line) {
			for (int k = 0; k < 10; ++k) {
				/*
				this->quads[((i - 20) * 10 + k) * 4].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 2 * 255));
				this->quads[((i - 20) * 10 + k) * 4 + 1].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 2 * 255));
				this->quads[((i - 20) * 10 + k) * 4 + 2].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 2 * 255));
				this->quads[((i - 20) * 10 + k) * 4 + 3].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 2 * 255));
				*/
				this->quads[((i - 20) * 10 + k) * 4].color = sf::Color(255, 255, 255, 255);
				this->quads[((i - 20) * 10 + k) * 4 + 1].color = sf::Color(255, 255, 255, 255);
				this->quads[((i - 20) * 10 + k) * 4 + 2].color = sf::Color(255, 255, 255, 255);
				this->quads[((i - 20) * 10 + k) * 4 + 3].color = sf::Color(255, 255, 255, 255);
			}
		}
	}
	else {
		for (auto & i : full_line) {
			for (int k = 0; k < 10; ++k) {
				this->quads[((i - 20) * 10 + k) * 4].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 10 * 255));
				this->quads[((i - 20) * 10 + k) * 4 + 1].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 10 * 255));
				this->quads[((i - 20) * 10 + k) * 4 + 2].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 10 * 255));
				this->quads[((i - 20) * 10 + k) * 4 + 3].color = sf::Color(255, 255, 255, (int)(std::max(0, (int)(0.5f - this->lineClearDelayTimer)) * 10 * 255));
			}
		}
	}


	// draw hold piece
	this->sprite_image_of_pieces.setPosition(0, 0);
	this->sprite_image_of_pieces.setTextureRect(sf::IntRect(this->pos_of_piece_in_image[this->holdPiece] * 4, 0, 4, 4));
	if (this->holdPiece != ' ')
		this->texture.draw(this->sprite_image_of_pieces);

	// draw preview next piece
	for (int i = 0; i < 5; i++) {
		this->sprite_image_of_pieces.setPosition(16, i * 4);
		this->sprite_image_of_pieces.setTextureRect(sf::IntRect(this->pos_of_piece_in_image[this->nextPiece[i]] * 4, 0, 4, 4));
		this->texture.draw(this->sprite_image_of_pieces);
	}

	this->texture.draw(this->quads);
	this->texture.display();
}

void Tetris::Board::render(sf::RenderTexture* _canvas)
{
	// draw red warning bars
	this->decor[0].position = sf::Vector2f(this->x + this->sprite.getScale().x * 4 + 8, this->y + this->sprite.getScale().x * (20 - this->garbage));
	this->decor[1].position = sf::Vector2f(this->x + this->sprite.getScale().x * 5 - 8, this->y + this->sprite.getScale().x * (20 - this->garbage));
	this->decor[2].position = sf::Vector2f(this->x + this->sprite.getScale().x * 5 - 8, this->y + this->sprite.getScale().x * 20);
	this->decor[3].position = sf::Vector2f(this->x + this->sprite.getScale().x * 4 + 8, this->y + this->sprite.getScale().x * 20);

	this->decor[4].position = sf::Vector2f(this->x + this->sprite.getScale().x * 15 + 8, this->y + this->sprite.getScale().x * (20 - this->garbage));
	this->decor[5].position = sf::Vector2f(this->x + this->sprite.getScale().x * 16 - 8, this->y + this->sprite.getScale().x * (20 - this->garbage));
	this->decor[6].position = sf::Vector2f(this->x + this->sprite.getScale().x * 16 - 8, this->y + this->sprite.getScale().x * 20);
	this->decor[7].position = sf::Vector2f(this->x + this->sprite.getScale().x * 15 + 8, this->y + this->sprite.getScale().x * 20);

	this->renderData();
	const sf::Texture& texture = this->texture.getTexture();
	this->sprite.setTexture(texture);
	_canvas->draw(this->sprite, sf::RenderStates(sf::BlendAdd));
	_canvas->draw(this->decor, sf::RenderStates(sf::BlendAdd));
}

std::string Tetris::Board::encodeData(std::array<std::array<int, 10>, 40> board_data)
{
	std::string result;
	for (int i = 0; i < 40; i++) {
		int a_i = 0;
		int b_i = 0;
		for (int k = 0; k < 5; k++) {
			a_i += (int)std::min(1, board_data[i][k]) * std::pow(2, k);
			b_i += (int)std::min(1, board_data[i][k + 5]) * std::pow(2, k);
		}
		char a_c = a_i;
		char b_c = b_i;
		result.push_back(a_c);
		result.push_back(b_c);
	}
	return result;
}

std::array<std::array<int, 10>, 40> Tetris::Board::decodeData(std::string code)
{
	std::array<std::array<int, 10>, 40> result;
	for (int i = 0; i < 40; i++) {
		int a_i = code[i * 2];
		int b_i = code[i * 2 + 1];
		for (int k = 0; k < 5; k++) {
			result[i][k] = a_i % 2;
			result[i][k + 5] = b_i % 2;
			a_i = a_i / 2;
			b_i = b_i / 2;
		}
	}
	return result;
}

void Tetris::Board::enableAI()
{
	this->realCurrentPiece.aiEnable = true;
	this->is_ai_enable = true;
}


