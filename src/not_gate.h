#ifndef NOT_GATE_H
#define NOT_GATE_H

#include "gate.h"
#include "pin.h"

class not_gate : public gate {
public:
	not_gate(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	std::vector<int> compute_value(int width, int pos) {
		// Skip first pin, since it's the output
		if (width != 1) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), NOT_COMPUTED);
			return value;
		}
		std::vector<int> value(1);
		if (pins.size() != 2) {
			value[0] = NOT_COMPUTED;
			return value;
		}
		value[0] = pins[1]->get_input_value()[0];
		if (value[0] == 0) {
			value[0] = ONE;
		} else if (value[0] == 1) {
			value[0] = ZERO;
		} else {
			value[0] = NOT_COMPUTED;
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
