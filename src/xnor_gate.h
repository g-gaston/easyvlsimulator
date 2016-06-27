#ifndef XNOR_GATE_H
#define XNOR_GATE_H

#include "gate.h"
#include "pin.h"

class xnor_gate : public gate {
public:
	xnor_gate(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	std::vector<int> compute_value(int width, int pos) {
		if (width != 1) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), NOT_COMPUTED);
			return value;
		}
		std::vector<int> value(1);
		int ones = 0;
		for (std::vector<pin *>::iterator pins_it = pins.begin() + 1
				; pins_it != pins.end(); ++pins_it) {
			int pin_value = (*pins_it)->get_input_value()[0];
			if (pin_value == ONE) {
				ones++;
			} else if (pin_value < 0) {
				value[0] = NOT_COMPUTED;
				return value;
			}
		}
		if (ones % 2 == 0) {
			value[0] = ONE;
		} else {
			value[0] = ZERO;
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
