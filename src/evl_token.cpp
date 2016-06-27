#include "evl_token.h"
#include <cstdlib>

Evl_token::Evl_token() {
	type = NAME;
	value = "";
	line = 0;
}

Evl_token::Evl_token(token_type t, std::string s, int l) {
	set(t, s, l);
}

void Evl_token::set(token_type t, std::string s, int l) {
	set_type(t);
	set_value(s);
	set_line(l);
}

void Evl_token::set_type(token_type t) {
	type = t;
}

void Evl_token::set_value(std::string s) {
	value = s;
}

void Evl_token::set_line(int l) {
	line = l;
}

bool Evl_token::is_single() {
	return (type == SINGLE);
}

bool Evl_token::is_name() {
	return (type == NAME);
}

bool Evl_token::is_number() {
	return (type == NUMBER);
}

std::string Evl_token::get_value() {
	return value;
}

int Evl_token::get_line() {
	return line;
}

int Evl_token::get_number() {
	if (type == NUMBER) {
		return atoi(value.c_str());
	} else {
		return -1;
	}
}

