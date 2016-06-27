#ifndef OR_GATE_H
#define OR_GATE_H

#include "gate.h"
#include "pin.h"

class or_gate : public gate {
public:
	or_gate(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	std::vector<int> compute_value(int width, int pos) {
		// Skip first pin, since it's the output
		if (width != 1) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), NOT_COMPUTED);
			return value;
		}
		std::vector<int> value(1);
		for (std::vector<pin *>::iterator pins_it = pins.begin() + 1
				; pins_it != pins.end(); ++pins_it) {
			int pin_value = (*pins_it)->get_input_value()[0];
			if (pin_value != 0) {
				value[0] = pin_value;
				return value;
			}
		}
		value[0] = 0;
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
