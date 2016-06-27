#include "pin.h"
#include "net.h"
#include "gate.h"

pin::pin() {
	name = "";
	width = 0;
	position = 0;
	set_pin_type(NONE);
	reset_pin_value();
	pin_value = std::vector<int>(width);
}

pin::pin(int w, gate* g, int p) {
	set_width(w);
	set_gate(g);
	set_position(p);
	set_pin_type(NONE);
	set_first_bit_in_bus(0);
	reset_pin_value();
	pin_value = std::vector<int>(width);
}

pin::pin(int w, gate* g, int p, int f) {
	set_width(w);
	set_gate(g);
	set_position(p);
	set_pin_type(NONE);
	set_first_bit_in_bus(f);
	reset_pin_value();
	pin_value = std::vector<int>(width);
}

pin::pin(std::string n, int w) {
	set_name(n);
	set_width(w);
}

void pin::set_name(std::string n) {
	name = n;
}

void pin::set_width(int w) {
	width= w;
}

void pin::set_gate(gate* ga) {
	g = ga;
}

void pin::add_net(net* nt) {
	nets.push_back(nt);
}

void pin::set_position(int pos) {
	position = pos;
}

void pin::set_pin_type(pin_type t){
	type = t;
}

void pin::set_first_bit_in_bus(int f){
	first_bit_in_bus = f;
}

void pin::reset_pin_value() {
	pin_value_status = EMPTY;
}

std::string pin::get_name() {
	return name;
}

int pin::get_width() {
	return width;
}

gate* pin::get_gate() {
	return g;
}

std::vector<net *> pin::get_nets() {
	return nets;
}

int pin::get_position() {
	return position;
}

int pin::get_output_value(int bus_pos) {
	if (type == OUT) {
		if (pin_value_status == EMPTY) {
			pin_value = g->compute_value(width, position);
			pin_value_status = COMPUTED;
		}
		int result_pos = bus_pos - first_bit_in_bus;
		if (result_pos >=0 && result_pos < width) {
			return pin_value[result_pos];
		}
	}
	return NOT_COMPUTED;
}

std::vector<int> pin::get_input_value() {
	std::vector<int> value (width);
	if (type == IN) {
		if (pin_value_status == EMPTY) {
			int pos = 0;
			std::vector<net *>::iterator nets_it;
			for (nets_it = nets.begin(), pos = 0;
				nets_it != nets.end(); ++nets_it, ++pos) {
				if (pos < width) {
					pin_value[pos] = (*nets_it)->get_value();
				} else {
					std::fill(pin_value.begin(), pin_value.end(), NOT_COMPUTED);
					return value;
				}
			}
			pin_value_status = COMPUTED;
		}
		return pin_value;
	}
	std::fill (pin_value.begin(), pin_value.end(), NOT_COMPUTED);
	return pin_value;
}
