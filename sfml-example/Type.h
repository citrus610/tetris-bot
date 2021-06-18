#pragma once

#include "../bot/bot.h"

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

static void set_weight_js(weight & w, json js, int player) {
	std::string plr = std::to_string(player);
	w.top_height = js[plr]["top_height"];
	w.top_height_top_half = js[plr]["top_height_top_half"];
	w.top_height_top_quarter = js[plr]["top_height_top_quarter"];
	w.bumpiness = js[plr]["bumpiness"];
	w.bumpiness_s = js[plr]["bumpiness_s"];
	w.well = js[plr]["well"];
	w.hole = js[plr]["hole"];
	w.blocked_cell = js[plr]["blocked_cell"];
	w.block_above_hole = js[plr]["block_above_hole"];
	w.block_above_hole_s = js[plr]["block_above_hole_s"];
	w.well_1_10 = js[plr]["well_1_10"];
	w.well_2_9 = js[plr]["well_2_9"];
	w.well_3_8 = js[plr]["well_3_8"];
	w.well_center = js[plr]["well_center"];
	for (int i = 0; i < 2; ++i) {
		w.structure[i] = js[plr]["structure"][i];
	}
	w.b2b_max_chain = js[plr]["b2b_max_chain"];
	w.ren_max_chain = js[plr]["ren_max_chain"];
	w.b2b_clear = js[plr]["b2b_clear"];
	w.ren_clear = js[plr]["ren_clear"];
	w.line_sent = js[plr]["line_sent"];
	w.perfect_clear = js[plr]["perfect_clear"];
	w.t_spin_1 = js[plr]["t_spin_1"];
	w.t_spin_2 = js[plr]["t_spin_2"];
	w.t_spin_3 = js[plr]["t_spin_3"];
	w.tetris = js[plr]["tetris"];
	w.burn_1 = js[plr]["burn_1"];
	w.burn_2 = js[plr]["burn_2"];
	w.burn_3 = js[plr]["burn_3"];
	w.waste_time = js[plr]["waste_time"];
	for (int i = 0; i < 2; ++i) {
		w.waste_structure[i] = js[plr]["waste_structure"][i];
	}
	w.waste_T = js[plr]["waste_T"];
	w.waste_I = js[plr]["waste_I"];
	w.hold_I = js[plr]["hold_I"];
	w.hold_T = js[plr]["hold_T"];
};