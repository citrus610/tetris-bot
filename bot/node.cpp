#include "node.h"

void node::attempt(node & child, bool hold, const std::vector<move>& path, vec16<piece>& next_queue)
{
	child = *this;
	child.score = 0;
	++child.depth;
	/*
	child.board = this->board;
	child.current = this->current;
	child.hold = this->hold;
	child.next = this->next;
	child.b2b = this->b2b;
	child.ren = this->ren;
	child.p_t_struct[0] = this->t_struct[0];
	child.p_t_struct[1] = this->t_struct[1];
	child.tspin_1 = this->tspin_1;
	child.tspin_2 = this->tspin_2;
	child.tspin_3 = this->tspin_3;
	child.tetris = this->tetris;
	child.pc = this->pc;
	child.burn_1 = this->burn_1;
	child.burn_2 = this->burn_2;
	child.burn_3 = this->burn_3;
	child.line_sent = this->line_sent;
	child.max_b2b_chain = this->max_b2b_chain;
	child.max_ren_chain = this->max_ren_chain;
	child.T_wasted = this->T_wasted;
	child.I_wasted = this->I_wasted;
	child.time_wasted = this->time_wasted;
	child.t_struct_wasted[0] = this->t_struct_wasted[0];
	child.t_struct_wasted[1] = this->t_struct_wasted[1];*/

	bitpiece s_piece;
	if (hold) {
		if (child.hold == PIECE_NONE) {
			child.hold = child.current;
			child.current = next_queue[child.next];
			++child.next;
		}
		else {
			piece value = child.hold;
			child.hold = child.current;
			child.current = value;
		}
	}
	s_piece.init(child.current);

	for (auto & step : path) {
		switch (step)
		{
		case MOVE_LEFT:
			child.board.try_left(s_piece);
			break;
		case MOVE_RIGHT:
			child.board.try_right(s_piece);
			break;
		case MOVE_CW:
			child.board.try_rotate(s_piece, 1);
			break;
		case MOVE_CCW:
			child.board.try_rotate(s_piece, 3);
			break;
		case MOVE_DOWN:
			child.board.try_down(s_piece);
			break;
		default:
			break;
		}
	}

	bool t_spin = child.board.t_spin(s_piece);
	child.board.place_piece(s_piece);
	int line_cleared = child.board.clear_line();
	int line_sent = child.board.line_sent(line_cleared, t_spin, child.b2b, child.ren);

	if (child.next < next_queue.size) {
		child.current = next_queue[child.next];
		++child.next;
	}
	else {
		child.current = PIECE_NONE;
	}

	if (t_spin) {
		switch (line_cleared)
		{
		case 0:
			child.lock = LOCK_NONE;
			break;
		case 1:
			child.lock = LOCK_TSPIN_1;
			++child.tspin_1;
			break;
		case 2:
			child.lock = LOCK_TSPIN_2;
			++child.tspin_2;
			break;
		case 3:
			child.lock = LOCK_TSPIN_3;
			++child.tspin_3;
			break;
		default:
			break;
		}
	}
	else {
		switch (line_cleared)
		{
		case 0:
			child.lock = LOCK_NONE;
			break;
		case 1:
			child.lock = LOCK_BURN_1;
			break;
		case 2:
			child.lock = LOCK_BURN_2;
			break;
		case 3:
			child.lock = LOCK_BURN_3;
			break;
		case 4:
			child.lock = LOCK_TETRIS;
			++child.tetris;
			break;
		default:
			break;
		}
	}

	if ((child.board.row[39] & 0b1111111111) == 0b0) {
		child.lock = LOCK_PC;
		++child.pc;
	}

	switch (child.lock)
	{
	case LOCK_BURN_1:
		++child.burn_1;
		break;
	case LOCK_BURN_2:
		++child.burn_2;
		break;
	case LOCK_BURN_3:
		++child.burn_3;
		break;
	default:
		break;
	}

	child.line_sent += line_sent;

	if (s_piece.type == PIECE_T && child.lock != LOCK_TSPIN_1 && child.lock != LOCK_TSPIN_2 && child.lock != LOCK_TSPIN_3 && child.lock != LOCK_PC)
		++child.T_wasted;
	if (s_piece.type == PIECE_I && child.lock != LOCK_TETRIS && child.lock != LOCK_PC && child.lock != LOCK_NONE)
		++child.I_wasted;

	if (child.b2b > child.max_b2b_chain) child.max_b2b_chain = child.b2b;
	if (child.ren > child.max_ren_chain) child.max_ren_chain = child.ren;
}
