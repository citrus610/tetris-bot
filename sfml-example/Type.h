#pragma once

#include "../bot/bot.h"
#include "../optimizer/optimizer.h"

static piece char_to_piece(char type) {
	switch (type)
	{
	case ' ':
		return PIECE_NONE;
		break;
	case 'I':
		return PIECE_I;
		break;
	case 'J':
		return PIECE_J;
		break;
	case 'L':
		return PIECE_L;
		break;
	case 'T':
		return PIECE_T;
		break;
	case 'Z':
		return PIECE_Z;
		break;
	case 'S':
		return PIECE_S;
		break;
	case 'O':
		return PIECE_O;
		break;
	default:
		return PIECE_NONE;
		break;
	}
}