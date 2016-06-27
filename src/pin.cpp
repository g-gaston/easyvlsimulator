#include "pin.h"
#include "net.h"
#include "gate.h"

pin::pin() {
	name = "";
	width = 0;
	position = 0;
	set_pin_type(NONE);
}

pin::pin(int w, gate* g, int p) {
	set_width(w);
	set_gate(g);
	set_position(p);
	set_pin_type(NONE);
	set_first_bit_in_bus(0);
}

pin::pin(int w, gate* g, int p, int f) {
	set_width(w);
	set_gate(g);
	set_position(p);
	set_pin_type(NONE);
	set_first_bit_in_bus(f);
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
		std::vector<int> value = g->compute_value(width, position);
		int result_pos = bus_pos - first_bit_in_bus;
		//std::cout << "result pos: " << itos(result_pos) << " first_bit_in_bus: " << itos(first_bit_in_bus) << " width: " << itos(width) << std::endl;
		if (result_pos >=0 && result_pos < width) {
			return value[result_pos];
		}
	}
	//std::cout << "NOT TYPE OUT, TYPE: ";
	if (type == IN)
	{
		//std::cout << "IN" << std::endl;
	} else if (type == NONE) {
		//std::cout << "NONE" << std::endl;
	} else if (type == OUT) {
		//std::cout << "OUT" << std::endl;
	}
	return NOT_COMPUTED;
}

std::vector<int> pin::get_input_value() {
	std::vector<int> value (width);
	if (type == IN) {
		int pos = 0;
		std::vector<net *>::iterator nets_it;
		for (nets_it = nets.begin(), pos = 0;
			nets_it != nets.end(); ++nets_it, ++pos) {
			if (pos < width) {
				value[pos] = (*nets_it)->get_value();
				//std::cout << "value[" << itos(pos) << "]: " << itos(value[pos]) << std::endl;
			} else {
				std::fill(value.begin(), value.end(), NOT_COMPUTED);
				return value;
			}
		}
		return value;
	}
	//std::cout << "invalid type output" << std::endl;
	std::fill (value.begin(), value.end(), NOT_COMPUTED);
	return value;
}
