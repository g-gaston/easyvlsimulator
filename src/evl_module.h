#ifndef EVL_MODULE_H
#define EVL_MODULE_H

#include <string>
#include <vector>
#include "evl_wire.h"
#include "evl_component.h"
#include "evl_assign.h"
#include "evl_port.h"

class Evl_module {
	std::string type;
	std::vector<Evl_wire> wires;
	std::vector<Evl_component> components;
	std::vector<Evl_assign> assignments;
	std::vector<Evl_port> ports;
public:
	Evl_module();
	Evl_module(std::string t);
	void set_type(std::string t);
	std::string get_type();
	std::vector<Evl_wire> const &get_wires() const;
	void add_wire(Evl_wire w);
	std::vector<Evl_component> const &get_components() const;
	void add_component(Evl_component c);
	void update_all_component_pins();
	std::vector<Evl_assign> const &get_assignments() const;
	void add_assign(Evl_assign w);
	std::vector<Evl_port> const &get_ports() const;
	void add_port(Evl_port p);
	void clear();
};

#endif
