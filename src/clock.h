#ifndef CLOCK_H
#define CLOCK_H

#include "gate.h"
#include "pin.h"

class clock_g : public gate {
public:
	clock_g(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	std::vector<int> compute_value(int width, int pos) {
		std::vector<int> value(1);
		return value;
	};
	bool is_output_pin(int pos) {
		return true;
	};

};

#endif
