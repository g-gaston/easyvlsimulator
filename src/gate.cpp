#include "gate.h"

gate::gate() {
	type = "";
	name = "";
	set_gate_type(UNDEXPECIFIED);
}

gate::gate(std::string t, std::string n) {
	set_type(t);
	set_name(n);
	set_gate_type(UNDEXPECIFIED);
}

gate::gate(std::string t, std::string n, gate_type g_t) {
	set_type(t);
	set_name(n);
	set_gate_type(g_t);
}

void gate::set_type(std::string t) {
	type = t;
}

void gate::set_name(std::string n) {
	name = n;
}

void gate::set_pin_num(int p) {
	pin_num = p;
}

void gate::set_gate_type(gate_type g_t) {
	gate_t = g_t;
}

std::string gate::get_type() {
	return type;
}

std::string gate::get_name() {
	return name;
}

int gate::get_pin_num() {
	return pin_num;
}

gate_type gate::get_gate_type() {
	return gate_t;
}

std::vector<pin *> gate::get_pins(){
	return pins;
}

void gate::add_pin(pin *w) {
	pins.push_back(w);
}

void gate::clear() {
	type = "";
	name = "";
	pins.clear();
}
