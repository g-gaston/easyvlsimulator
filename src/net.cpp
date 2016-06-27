#include "net.h"
#include "pin.h"
#include "gate.h"

net::net() {
	name = "";
	reset_value();
	set_bus_pos(0);
	driver = NOT_DETERMINED;
}

net::net(std::string n) {
	set_name(n);
	reset_value();
	set_bus_pos(0);
	driver = NOT_DETERMINED;
}

net::net(std::string n, int p) {
	set_name(n);
	reset_value();
	set_bus_pos(p);
	driver = NOT_DETERMINED;
}

void net::set_name(std::string n) {
	name = n;
}

void net::reset_value() {
	value = NOT_COMPUTED;
}

void net::set_bus_pos(int p) {
	bus_pos = p;
}

void net::set_driver_pin(pin *p) {
	if (driver == NOT_DETERMINED) {
		if (p->get_gate()->get_gate_type() == TRIS || p->get_gate()->get_gate_type() == BUF) {
			driver = TRI_STATE;
			tri_driver_pins.push_back(p);
		} else {
			driver = UNIQUE;
			driver_pin = p;
		}
	} else if (driver == UNIQUE) {
		if (p->get_gate()->get_gate_type() == TRIS || p->get_gate()->get_gate_type() == BUF) {
			driver = TRI_STATE;
			tri_driver_pins.push_back(driver_pin);
			driver_pin = NULL;
			tri_driver_pins.push_back(p);
		}
	} else if (driver == TRI_STATE) {
		tri_driver_pins.push_back(p);
	}
}

std::string net::get_name() {
	return name;
}

std::vector<pin *>net::get_pins() {
	return pins;
}

void net::add_pin(pin* w) {
	pins.push_back(w);
}

void net::clear() {
	name = "";
	pins.clear();
}

int net::get_bus_pos() {
	return bus_pos;
}

int net::get_value() {
	if (value == NOT_COMPUTED) {
		if (driver == UNIQUE) {
			//std::cout << "DRIVER UNIQUE" << std::endl;
			value = driver_pin->get_output_value(bus_pos);
		} else if (driver == TRI_STATE) {
			//std::cout << "DRIVER TRI" << std::endl;
			int first_computed_value = NOT_COMPUTED;
			for (std::vector<pin *>::iterator pins_it = tri_driver_pins.begin()
				; pins_it != tri_driver_pins.end(); ++pins_it) {
				value = (*pins_it)->get_output_value(bus_pos);
				if (value == NOT_COMPUTED) {
					return value;
				}
				if (first_computed_value == NOT_COMPUTED) {
					first_computed_value = value;
				} else if (first_computed_value == Z) {
					if (value == ZERO || value == ONE) {
						first_computed_value = value;
					}
				} else {
					if (value == ZERO || value == ONE) {
						value = NOT_COMPUTED;
						return value;
					}
				}
			}
			value = first_computed_value;
		} else {
			//std::cout << "DRIVER ??" << std::endl;
		}

	}
	return value;
}

