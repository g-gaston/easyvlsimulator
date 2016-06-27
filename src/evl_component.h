#ifndef EVL_COMPONENT_H
#define EVL_COMPONENT_H

#include <string>
#include <vector>
#include "evl_pin.h"

class Evl_component {
	std::string type;
	std::string name;
	std::vector<Evl_pin> pins;

public:
	Evl_component();
	Evl_component(std::string t, std::string n);
	void set_type(std::string t);
	void set_name(std::string n);
	std::string get_type();
	std::string get_name();
	std::vector<Evl_pin> *get_pins_pointer();
	const std::vector<Evl_pin> &get_pins() const;
	void add_pin(Evl_pin w);
	void clear();
	void link_back_pins();
};

#endif
