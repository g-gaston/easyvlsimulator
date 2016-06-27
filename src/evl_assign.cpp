#include "evl_assign.h"

Evl_assign::Evl_assign() {
	lhs_name = "";
	rhs_name = "";
	lhs_msb = -1;
	lhs_lsb = -1;
	rhs_msb = -1;
	rhs_lsb = -1;
}


void Evl_assign::set_lhs_name(std::string n) {
	lhs_name = n;
}

std::string Evl_assign::get_lhs_name() {
	return lhs_name;
}

void Evl_assign::set_lhs_msb(int m) {
	lhs_msb = m;
}

void Evl_assign::set_lhs_lsb(int l) {
	lhs_lsb = l;
}

int Evl_assign::get_lhs_msb() {
	return lhs_msb;
}

int Evl_assign::get_lhs_lsb() {
	return lhs_lsb;
}

void Evl_assign::set_rhs_name(std::string n) {
	rhs_name = n;
}

std::string Evl_assign::get_rhs_name() {
	return rhs_name;
}

void Evl_assign::set_rhs_msb(int m) {
	rhs_msb = m;
}

void Evl_assign::set_rhs_lsb(int l) {
	rhs_lsb = l;
}

int Evl_assign::get_rhs_msb() {
	return rhs_msb;
}

int Evl_assign::get_rhs_lsb() {
	return rhs_lsb;
}
