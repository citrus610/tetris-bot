#ifndef SFML_TYPE_H
#define SFML_TYPE_H

#include <iomanip>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "bot.h"

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

static void create_init_js() {
	// check if exist?
	std::ifstream f("init.json");
	if (f.good()) {
		return;
	};
	f.close();


	json j_file;

	weight w;

	for (int i = 1; i < 3; ++i) {
		j_file[std::to_string(i)]["top_height"] = w.top_height;
		j_file[std::to_string(i)]["top_height_top_half"] = w.top_height_top_half;
		j_file[std::to_string(i)]["top_height_top_quarter"] = w.top_height_top_quarter;
		j_file[std::to_string(i)]["bumpiness"] = w.bumpiness;
		j_file[std::to_string(i)]["bumpiness_s"] = w.bumpiness_s;
		j_file[std::to_string(i)]["well"] = w.well;
		j_file[std::to_string(i)]["hole"] = w.hole;
		j_file[std::to_string(i)]["blocked_cell"] = w.blocked_cell;
		j_file[std::to_string(i)]["block_above_hole"] = w.block_above_hole;
		j_file[std::to_string(i)]["block_above_hole_s"] = w.block_above_hole_s;
		j_file[std::to_string(i)]["well_1_10"] = w.well_1_10;
		j_file[std::to_string(i)]["well_2_9"] = w.well_2_9;
		j_file[std::to_string(i)]["well_3_8"] = w.well_3_8;
		j_file[std::to_string(i)]["well_center"] = w.well_center;
		j_file[std::to_string(i)]["structure"] =
		{
			w.structure[0],
			w.structure[1]
		};
		j_file[std::to_string(i)]["b2b_max_chain"] = w.b2b_max_chain;
		j_file[std::to_string(i)]["ren_max_chain"] = w.ren_max_chain;
		j_file[std::to_string(i)]["b2b_chain"] = w.b2b_chain;
		j_file[std::to_string(i)]["ren_chain"] = w.ren_chain;
		j_file[std::to_string(i)]["line_sent"] = w.line_sent;
		j_file[std::to_string(i)]["perfect_clear"] = w.perfect_clear;
		j_file[std::to_string(i)]["t_spin_1"] = w.t_spin_1;
		j_file[std::to_string(i)]["t_spin_2"] = w.t_spin_2;
		j_file[std::to_string(i)]["t_spin_3"] = w.t_spin_3;
		j_file[std::to_string(i)]["tetris"] = w.tetris;
		j_file[std::to_string(i)]["burn_1"] = w.burn_1;
		j_file[std::to_string(i)]["burn_2"] = w.burn_2;
		j_file[std::to_string(i)]["burn_3"] = w.burn_3;
		j_file[std::to_string(i)]["waste_time"] = w.waste_time;
		j_file[std::to_string(i)]["waste_structure"] =
		{
			w.waste_structure[0],
			w.waste_structure[1]
		};
		j_file[std::to_string(i)]["waste_T"] = w.waste_T;
		j_file[std::to_string(i)]["waste_I"] = w.waste_I;
		j_file[std::to_string(i)]["hold_T"] = w.hold_T;
		j_file[std::to_string(i)]["hold_I"] = w.hold_I;
	}

	std::ofstream o("init.json");
	o << std::setw(4) << j_file << std::endl;
	o.close();
};

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
	w.b2b_chain = js[plr]["b2b_chain"];
	w.ren_chain = js[plr]["ren_chain"];
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

static bot_data board_to_bot_data(tetris_board & _board) {
	bot_data result;

	result.current = char_to_piece(_board.current_piece);
	result.hold = char_to_piece(_board.hold_piece);
	result.b2b = _board.b2b;
	result.ren = _board.ren;
	for (auto & p : _board.next_piece) {
		result.next.add(char_to_piece(p));
	}
	for (int i = 0; i < 40; ++i) {
		uint16_t a_row = 0b0;
		for (int k = 0; k < 10; ++k) {
			if (_board.data[i][k] > 0) {
				a_row = a_row | (0b1 << (9 - k));
			}
		}
		result.board.row[i] = a_row;
	}

	return result;
};

static node board_to_node(tetris_board & _board) {
	node result;

	result.current = char_to_piece(_board.current_piece);
	result.hold = char_to_piece(_board.hold_piece);
	result.b2b = _board.b2b;
	result.ren = _board.ren;
	result.next = 0;
	for (int i = 0; i < 40; ++i) {
		uint16_t a_row = 0b0;
		for (int k = 0; k < 10; ++k) {
			if (_board.data[i][k] > 0) {
				a_row = a_row | (0b1 << (9 - k));
			}
		}
		result.board.row[i] = a_row;
	}

	return result;
}

#endif