#include "evl_module.h"

Evl_module::Evl_module() {
	type = "";
}

Evl_module::Evl_module(std::string t) {
	set_type(t);
}

void Evl_module::set_type(std::string t) {
	type = t;
}

std::string Evl_module::get_type() {
	return type;
}

std::vector<Evl_wire> const &Evl_module::get_wires() const {
	return wires;
}

void Evl_module::add_wire(Evl_wire w) {
	wires.push_back(w);
}

std::vector<Evl_component> const &Evl_module::get_components() const {
	return components;
}

void Evl_module::add_component(Evl_component c) {
	components.push_back(c);
}

void Evl_module::update_all_component_pins() {
	for (std::vector<Evl_component>::iterator components_it = components.begin()
		; components_it != components.end(); ++components_it) {
		(*components_it).link_back_pins();
	}
}

std::vector<Evl_assign> const &Evl_module::get_assignments() const {
	return assignments;
}

void Evl_module::add_assign(Evl_assign c) {
	assignments.push_back(c);
}

std::vector<Evl_port> const &Evl_module::get_ports() const {
	return ports;
}

void Evl_module::add_port(Evl_port p) {
	ports.push_back(p);
}

void Evl_module::clear() {
	set_type("");
	wires.clear();
	components.clear();
	assignments.clear();
	ports.clear();
}
