#include "definition.h"

void weight::set(const std::string & file_path)
{
	std::ifstream file;
	file.open(file_path);
	json js;
	file >> js;

	this->top_height = js["top_height"];
	this->top_height_top_half = js["top_height_top_half"];
	this->top_height_top_quarter = js["top_height_top_quarter"];
	this->bumpiness = js["bumpiness"];
	this->bumpiness_s = js["bumpiness_s"];
	this->well = js["well"];
	this->hole = js["hole"];
	this->blocked_cell = js["blocked_cell"];
	this->block_above_hole = js["block_above_hole"];
	this->block_above_hole_s = js["block_above_hole_s"];
	this->well_1_10 = js["well_1_10"];
	this->well_2_9 = js["well_2_9"];
	this->well_3_8 = js["well_3_8"];
	this->well_center = js["well_center"];
	for (int i = 0; i < 3; ++i) {
		this->structure[0] = js["structure"][0];
	}
	this->b2b_max_chain = js["b2b_max_chain"];
	this->ren_max_chain = js["ren_max_chain"];
	this->b2b_clear = js["b2b_clear"];
	this->ren_clear = js["ren_clear"];
	this->line_sent = js["line_sent"];
	this->perfect_clear = js["perfect_clear"];
	this->t_spin_1 = js["t_spin_1"];
	this->t_spin_2 = js["t_spin_2"];
	this->t_spin_3 = js["t_spin_3"];
	this->tetris = js["tetris"];
	this->burn_1 = js["burn_1"];
	this->burn_2 = js["burn_2"];
	this->burn_3 = js["burn_3"];
	this->waste_time = js["waste_time"];
	for (int i = 0; i < 3; ++i) {
		this->waste_structure[0] = js["waste_structure"][0];
	}
	this->waste_T = js["waste_T"];
	this->waste_I = js["waste_I"];
	this->hold_I = js["hold_I"];
	this->hold_T = js["hold_T"];

	file.close();
}
