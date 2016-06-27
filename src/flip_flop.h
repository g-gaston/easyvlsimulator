#ifndef FLIP_FLOP_H
#define FLIP_FLOP_H

#include "gate.h"
#include "pin.h"

class flip_flop : public gate {
	int past_state;
	int present_state;
public:
	flip_flop(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) {
		past_state = ZERO;
		present_state = NOT_COMPUTED;
	};
	std::vector<int> compute_value(int width, int pos) {
		if (width != 1) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), NOT_COMPUTED);
			return value;
		}
		std::vector<int> value(1);
		value[0] = present_state;
		return value;
	};
	bool is_output_pin(int pos) {
		if (pos != 0) {
			return false;
		} else {
			return true;
		}
	};
	void clock_tick() {
		present_state = past_state;
		past_state = NOT_COMPUTED;
	}
	void get_new_state() {
		if (pins.size() < 2) {
			return;
		}
		past_state = pins[1]->get_input_value()[0];
	}
};

#endif
