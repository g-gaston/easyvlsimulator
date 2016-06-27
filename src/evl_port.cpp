#include "evl_port.h"

Evl_port::Evl_port() {
	type = "";
	width = 0;
}

Evl_port::Evl_port(std::string t, int l) {
	set_type(t);
	set_width(l);
}

void Evl_port::set_type(std::string t) {
	type = t;
	if (type == "input") {
		type_ = PORT_IN;
	} else if (type == "output") {
		type_ = PORT_OUT;
	}
}

void Evl_port::set_width(int l) {
	width = l;
}

std::string Evl_port::get_type() {
	return type;
}

port_type Evl_port::get_port_type() {
	return type_;
}


int Evl_port::get_width() {
	return width;
}

void Evl_port::set_name(std::string t) {
	name = t;
}
std::string Evl_port::get_name() {
	return name;
}

