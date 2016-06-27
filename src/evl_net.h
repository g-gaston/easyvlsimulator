#ifndef EVL_NET_H
#define EVL_NET_H

#include <string>
#include <vector>
class Evl_pin;

class Evl_net {
	std::string name;
	std::vector<Evl_pin*> pins;

public:
	Evl_net();
	Evl_net(std::string n);
	void set_name(std::string n);
	std::string get_name();
	std::vector<Evl_pin*> const &get_pins() const;
	void add_pin(Evl_pin* w);
	void clear();
};

#endif
