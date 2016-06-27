#ifndef TRIS_H
#define TRIS_H

#include "gate.h"
#include "pin.h"

class tris : public gate {
public:
	tris(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	std::vector<int> compute_value(int width, int pos) {
		if (width != 1) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), NOT_COMPUTED);
			return value;
		}
		std::vector<int> value(1);
		int pin_value = pins[2]->get_input_value()[0];
		if (pin_value == ONE) {
			value[0] = pins[1]->get_input_value()[0];
		} else {
			value[0] = Z;
		}
		return value;
	};
	bool is_output_pin(int pos) {
		if (pos != 0) {
			return false;
		} else {
			return true;
		}
	};

};

#endif
