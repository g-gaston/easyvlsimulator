#ifndef PIN_H
#define PIN_H

#include <string>
#include <vector>
#include <iostream>
#include "simlib.h"

class gate;
class net;

enum pin_type{IN, OUT, NONE};
enum value_status{COMPUTED, EMPTY};

class pin {
	std::string name;
	int width;
	gate* g;
	std::vector<net *> nets;
	int position;
	pin_type type;
	int first_bit_in_bus;
	std::vector<int> pin_value;
	value_status pin_value_status;
public:
	pin();
	pin(int w, gate* g, int p);
	pin(int w, gate* g, int p, int f);
	pin(std::string t, int w);
	void set_name(std::string n);
	void set_width(int m);
	void set_gate(gate* g);
	void add_net(net* nt);
	void set_position(int pos);
	void set_pin_type(pin_type t);
	void set_first_bit_in_bus(int f);
	void reset_pin_value();
	std::string get_name();
	int get_width();
	gate* get_gate();
	std::vector<net *> get_nets();
	int get_position();
	int get_output_value(int bus_pos);
	std::vector<int> get_input_value();
};

#endif
