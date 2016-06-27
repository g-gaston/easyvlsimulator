#include "evl_net.h"

Evl_net::Evl_net() {
	name = "";
}

Evl_net::Evl_net(std::string n) {
	set_name(n);
}

void Evl_net::set_name(std::string n) {
	name = n;
}

std::string Evl_net::get_name() {
	return name;
}

std::vector<Evl_pin*> const &Evl_net::get_pins() const {
	return pins;
}

void Evl_net::add_pin(Evl_pin* w) {
	pins.push_back(w);
}

void Evl_net::clear() {
	name = "";
	pins.clear();
}

