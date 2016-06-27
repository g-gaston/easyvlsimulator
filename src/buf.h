#ifndef BUF_H
#define BUF_H

#include "gate.h"
#include "pin.h"

class buf : public gate {
public:
	buf(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
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
