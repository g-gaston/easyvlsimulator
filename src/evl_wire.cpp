#include "evl_wire.h"

Evl_wire::Evl_wire() {
	name = "";
	width = 0;
}

Evl_wire::Evl_wire(std::string t, int l) {
	set_name(t);
	set_width(l);
}

void Evl_wire::set_name(std::string t) {
	name = t;
}

void Evl_wire::set_width(int l) {
	width = l;
}

std::string Evl_wire::get_name() {
	return name;
}

int Evl_wire::get_width() {
	return width;
}

