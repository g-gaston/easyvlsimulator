#include "evl_pin.h"

Evl_pin::Evl_pin() {
	name = "";
	msb = -1;
	lsb = -1;
	position = 0;
}

Evl_pin::Evl_pin(std::string n, int m, int l) {
	set_name(n);
	set_msb(m);
	set_lsb(l);
}

void Evl_pin::set_name(std::string n) {
	name = n;
}

void Evl_pin::set_msb(int m) {
	msb = m;
}

void Evl_pin::set_lsb(int l) {
	lsb = l;
}

void Evl_pin::set_component(Evl_component* com) {
	component = com;
}

void Evl_pin::set_net(Evl_net* nt) {
	net = nt;
}

void Evl_pin::set_position(int pos) {
	position = pos;
}

std::string Evl_pin::get_name() {
	return name;
}

int Evl_pin::get_msb() {
	return msb;
}

int Evl_pin::get_lsb() {
	return lsb;
}

Evl_component* Evl_pin::get_component() {
	return component;
}

Evl_net* Evl_pin::get_net() {
	return net;
}

int Evl_pin::get_position() {
	return position;
}
