#ifndef EVL_PORT_H
#define EVL_PORT_H

#include <string>

enum port_type{PORT_IN, PORT_OUT};

class Evl_port {
	std::string type;
	port_type type_;
	std::string name;
	int width;
public:
	Evl_port();
	Evl_port(std::string t, int l);
	void set_type(std::string t);
	void set_name(std::string t);
	void set_width(int l);
	std::string get_type();
	port_type get_port_type();
	std::string get_name();
	int get_width();
};

#endif
