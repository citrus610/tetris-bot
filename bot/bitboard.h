#pragma once

#include "definition.h"

struct bitpiece
{
	piece type;
	int x = 0, y = 0, current_form = 0;

	void init(piece type) {
		y = 18;
		this->type = type;
		current_form = 0;
		x = piece_init_pos[type];
	};
};

struct bitboard
{
	uint16_t row[40];

	uint16_t & operator [] (unsigned int index) {
		return row[index];
	};

	bool operator == (const bitboard& other) {
		for (int i = 39; i >= 0; --i) {
			if (row[i] != other.row[i]) {
				return false;
			}
			if (row[i] == 0b0 && other.row[i] == 0b0) {
				break;
			}
		}
		return true;
	};

	bool is_occupying(int x, int y);
	bool is_colliding(bitpiece& piece, int x, int y);
	bool t_spin(bitpiece& piece);
	void place_piece(bitpiece& piece);
	int clear_line();
	int line_sent(int line_cleared, bool t_spin, int& b2b, int& ren);
	void try_left(bitpiece& piece);
	void try_right(bitpiece& piece);
	void try_down(bitpiece& piece);
	void try_rotate(bitpiece& piece, int direction);
};

inline bool bitboard::is_occupying(int x, int y) {
	if (x < 0 || x > 9 || y < 0 || y > 39) {
		return true;
	}
	else {
		return (row[y] >> (9 - x)) & 0b1;
	}
};

inline bool bitboard::is_colliding(bitpiece & piece, int x, int y)
{
	for (int i = 0; i < 4; ++i) {
		uint16_t piece_chunk = (piece_form[piece.type][piece.current_form] >> (4 * (3 - i))) & 0b1111;

		if (i + y > 39) {
			if (piece_chunk != 0b0) return true;
		}
		else {
			uint16_t board_chunk = (row[i + y] << 3) | 0b1110000000000111;
			if (x < 0)
				piece_chunk = piece_chunk << 9 << -x;
			else
				piece_chunk = piece_chunk << 9 >> x;
			if (board_chunk & piece_chunk)
				return true;
		}
	}
	return false;
};

inline bool bitboard::t_spin(bitpiece& piece) {
	if (piece.type == PIECE_T && piece.current_form != 0) {
		if (piece.current_form == 2) {
			return (is_occupying(piece.x, piece.y) && is_occupying(piece.x - 1, piece.y + 1) && is_occupying(piece.x, piece.y + 2) && is_occupying(piece.x + 2, piece.y + 2)) ||
				(is_occupying(piece.x + 2, piece.y) && is_occupying(piece.x + 3, piece.y + 1) && is_occupying(piece.x, piece.y + 2) && is_occupying(piece.x + 2, piece.y + 2));
		}
		else if (piece.current_form == 1) {
			return is_colliding(piece, piece.x - 1, piece.y) && is_colliding(piece, piece.x + 1, piece.y) && is_colliding(piece, piece.x, piece.y - 1) && is_occupying(piece.x + 2, piece.y);
		}
		else if (piece.current_form == 3) {
			return is_colliding(piece, piece.x - 1, piece.y) && is_colliding(piece, piece.x + 1, piece.y) && is_colliding(piece, piece.x, piece.y - 1) && is_occupying(piece.x, piece.y);
		}
	}
	else {
		return false;
	}
};

inline void bitboard::place_piece(bitpiece & piece)
{
	for (int i = 0; i < 4; ++i) {
		uint16_t piece_chunk = (piece_form[piece.type][piece.current_form] >> (4 * (3 - i))) & 0b1111;
		if (piece.x < 0)
			row[i + piece.y] = (row[i + piece.y] | ((piece_chunk << 6) << -piece.x)) & 0b1111111111;
		else
			row[i + piece.y] = (row[i + piece.y] | ((piece_chunk << 6) >> piece.x)) & 0b1111111111;
	}
};

inline int bitboard::clear_line() {
	int line_cleared = 0;

	for (int i = 20; i < 40; ++i) {
		if ((row[i] & 0b1111111111) == 0b1111111111) {
			++line_cleared;
			for (int k = i; k > 0; --k) {
				row[k] = row[k - 1];
				if ((row[k] & 0b1111111111) == 0b0) break;
			}
		}
	}

	return line_cleared;
};

inline int bitboard::line_sent(int line_cleared, bool t_spin, int& b2b, int& ren) {
	int garbage = 0;

	if (t_spin) {
		garbage = line_cleared * 2;
		if (b2b > 0)
			++garbage;
		++b2b;
	}
	else {
		switch (line_cleared)
		{
		case 0:
			break;
		case 4:
			garbage = 4;
			if (b2b > 0)
				garbage++;
			++b2b;
			break;
		default:
			b2b = 0;
			garbage = line_cleared - 1;
			break;
		}
	}

	// ren
	if (line_cleared > 0) {
		ren++;
	}
	else {
		ren = 0;
	}
	if (ren < 10) {
		garbage += ren / 2;
	}
	else if (ren == 10) {
		garbage += 4;
	}
	else {
		garbage += 5;
	}

	// find if perfect clear
	if ((this->row[39] & 0b1111111111) == 0b0) {
		garbage = 10;
	}

	return garbage;
};

inline void bitboard::try_left(bitpiece & piece)
{
	if (!is_colliding(piece, piece.x - 1, piece.y)) --piece.x;
};

inline void bitboard::try_right(bitpiece & piece)
{
	if (!is_colliding(piece, piece.x + 1, piece.y)) ++piece.x;
};

inline void bitboard::try_down(bitpiece & piece)
{
	while (!is_colliding(piece, piece.x, piece.y + 1))
	{
		++piece.y;
	}
};

inline void bitboard::try_rotate(bitpiece & piece, int direction)
{
	/*
	direction = 1 -> rotate right
	direction = 3 -> rotate left
	*/
	int oldForm = piece.current_form;
	piece.current_form = (piece.current_form + direction) % (piece_form[piece.type].size());
	int k = 0;
	switch (piece.type)
	{
	case PIECE_I:
		if ((oldForm == 0 && piece.current_form == 1) || (oldForm == 3 && piece.current_form == 2)) //0 > R && L > 2
			k = 0;
		else if ((oldForm == 1 && piece.current_form == 0) || (oldForm == 2 && piece.current_form == 3)) //R > 0 && 2 > L
			k = 1;
		else if ((oldForm == 1 && piece.current_form == 2) || (oldForm == 0 && piece.current_form == 3)) //R > 2 && 0 > L
			k = 2;
		else if ((oldForm == 2 && piece.current_form == 1) || (oldForm == 3 && piece.current_form == 0)) //2 > R && L > 0
			k = 3;
		break;
	case PIECE_O:
		break;
	default:
		if (piece.current_form == 1) //From smt to R
			k = 0;
		else if (piece.current_form == 3) //From smt to L
			k = 2;
		else
			if (oldForm == 1) //From R to smt
				k = 1;
			else if (oldForm == 3) //From R to smt
				k = 3;
		break;
	}
	if (piece.type != PIECE_O) {
		if (this->is_colliding(piece, piece.x, piece.y)) {
			int c = 0;
			for (int i = 0; i < 4; ++i) {
				if (!this->is_colliding(piece, piece.x + piece_srs[piece.type][k][i][0], piece.y + piece_srs[piece.type][k][i][1])) {
					piece.x += piece_srs[piece.type][k][i][0];
					piece.y += piece_srs[piece.type][k][i][1];
					break;
				}
				c++;
			}
			if (c == 4) {
				piece.current_form = oldForm;
			}
		}
	}
};