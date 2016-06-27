#ifndef GATE_H
#define GATE_H

#include <string>
#include <vector>
#include "simlib.h"

class pin;

enum gate_type{AND, OR, XOR, XNOR, NOT, BUF, FLIP_FLOP, TRIS,
	CLOCK, ONES, ZEROS, INPUT, OUTPUT, LOOK_UP_TABLE, UNDEXPECIFIED};

class gate {
	std::string type;
	std::string name;
	gate_type gate_t;
	int pin_num;
protected:
	std::vector<pin *> pins;
public:
	gate();
	gate(std::string t, std::string n);
	gate(std::string t, std::string n, gate_type g_t);
	void set_type(std::string t);
	void set_name(std::string n);
	void set_pin_num(int p);
	void set_gate_type(gate_type g_t);
	std::string get_type();
	std::string get_name();
	int get_pin_num();
	std::vector<pin *> get_pins();
	void add_pin(pin *w);
	void clear();
	gate_type get_gate_type();
	virtual std::vector<int> compute_value(int width, int pos) = 0;
	virtual bool is_output_pin(int pos) = 0;
	virtual ~gate(){};
};

#endif
