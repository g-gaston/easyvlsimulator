#ifndef ONES_H
#define ONES_H

#include "gate.h"
#include "pin.h"

class ones : public gate {
public:
	ones(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	std::vector<int> compute_value(int width, int pos) {
		std::vector<int> value(width);
		std::fill(value.begin(), value.end(), ONE);
		return value;
	};
	bool is_output_pin(int pos) {
		return true;
	};

};

#endif
