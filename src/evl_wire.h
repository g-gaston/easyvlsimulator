#ifndef EVL_WIRE_H
#define EVL_WIRE_H

#include <string>

class Evl_wire {
	std::string name;
	int width;
public:
	Evl_wire();
	Evl_wire(std::string t, int l);
	void set_name(std::string t);
	void set_width(int l);
	std::string get_name();
	int get_width();
};

#endif
