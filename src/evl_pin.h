#ifndef EVL_PIN_H
#define EVL_PIN_H

#include <string>

class Evl_component;
class Evl_net;

class Evl_pin {
	std::string name;
	int msb;
	int lsb;
	Evl_component* component;
	Evl_net* net;
	int position;
public:
	Evl_pin();
	Evl_pin(std::string t, int m, int l);
	void set_name(std::string n);
	void set_msb(int m);
	void set_lsb(int l);
	void set_component(Evl_component* com);
	void set_net(Evl_net* nt);
	void set_position(int pos);
	std::string get_name();
	int get_msb();
	int get_lsb();
	Evl_component* get_component();
	Evl_net* get_net();
	int get_position();
};

#endif
