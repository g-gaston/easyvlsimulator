#ifndef NET_H
#define NET_H

#include <string>
#include <vector>
#include "simlib.h"

class pin;
class gate;

enum driver_type{UNIQUE, TRI_STATE, NOT_DETERMINED};

class net {
	std::string name;
	int bus_pos;
	std::vector<pin *> pins;
	int value;
	driver_type driver;
	pin *driver_pin;
	std::vector<pin *> tri_driver_pins;

public:
	net();
	net(std::string n);
	net(std::string n, int p);
	void set_name(std::string n);
	void reset_value();
	void set_bus_pos(int p);
	void set_driver_pin(pin *g);
	std::string get_name();
	std::vector<pin *> get_pins();
	void add_pin(pin* w);
	void clear();
	int get_value();
	int get_bus_pos();
};

#endif
