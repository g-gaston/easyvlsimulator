#include "evl_component.h"

Evl_component::Evl_component() {
	type = "";
	name = "";
}

Evl_component::Evl_component(std::string t, std::string n) {
	set_type(t);
	set_name(n);
}

void Evl_component::set_type(std::string t) {
	type = t;
}

void Evl_component::set_name(std::string n) {
	name = n;
}

std::string Evl_component::get_type() {
	return type;
}

std::string Evl_component::get_name() {
	return name;
}

std::vector<Evl_pin> *Evl_component::get_pins_pointer() {
	return &pins;
}

const std::vector<Evl_pin> &Evl_component::get_pins() const {
	return pins;
}

void Evl_component::add_pin(Evl_pin w) {
	pins.push_back(w);
}

void Evl_component::clear() {
	type = "";
	name = "";
	pins.clear();
}

void Evl_component::link_back_pins() {
	for (std::vector<Evl_pin>::iterator pins_it = pins.begin()
	; pins_it != pins.end(); ++pins_it) {
		(*pins_it).set_component(this);
	}
}
