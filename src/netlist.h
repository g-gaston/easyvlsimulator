#ifndef NETLIST_H
#define NETLIST_H

#include <string>
#include <vector>
#include "gate.h"
#include "net.h"
#include "pin.h"

#include "and_gate.h"
#include "or_gate.h"
#include "xor_gate.h"
#include "not_gate.h"
#include "buf.h"
#include "flip_flop.h"
#include "tris.h"
#include "clock.h"
#include "ones.h"
#include "zeros.h"
#include "input_gate.h"
#include "output_gate.h"
#include "look_up_table.h"
#include "xnor_gate.h"

class netlist {
public:
	std::vector<gate *> gates;
	std::vector<net *> nets;
	std::vector<pin *> pins;
	std::vector<output_gate *> out_gates;
	std::vector<flip_flop *> ff_gates;
	std::vector<input_gate *> in_gates;
	~netlist() {
	    for (std::vector<gate *>::iterator gates_it = gates.begin()
			; gates_it != gates.end(); ++gates_it) {
	    	(*(*gates_it)).~gate();
	    }
        for (std::vector<net *>::iterator nets_it = nets.begin()
    		; nets_it != nets.end(); ++nets_it) {
        	(*(*nets_it)).~net();
        }
    	for (std::vector<pin *>::iterator pins_it = pins.begin()
    		; pins_it != pins.end(); ++pins_it) {
        	(*(*pins_it)).~pin();
        }
	};
	gate *create_gate(std::string t, std::string n, std::string file_name) {
		gate *g;
		if (t == "and") {
			g = new and_gate(t, n, AND);
			gates.push_back(g);
			return g;
		} else if (t == "or") {
			g = new or_gate(t, n, OR);
			gates.push_back(g);
			return g;
		} else if (t == "xor") {
			g = new xor_gate(t, n, XOR);
			gates.push_back(g);
			return g;
		} else if (t == "xnor") {
			g = new xnor_gate(t, n, XNOR);
			gates.push_back(g);
			return g;
		} else if (t == "not") {
			g = new not_gate(t, n, NOT);
			gates.push_back(g);
			return g;
		} else if (t == "buf") {
			g = new buf(t, n, BUF);
			gates.push_back(g);
			return g;
		} else if (t == "evl_dff") {
			g = new flip_flop(t, n, FLIP_FLOP);
			gates.push_back(g);
			ff_gates.push_back((flip_flop *)g);
			return g;
		} else if (t == "tris") {
			g = new tris(t, n, TRIS);
			gates.push_back(g);
			return g;
		} else if (t == "evl_clock") {
			g = new clock_g(t, n, CLOCK);
			gates.push_back(g);
			return g;
		} else if (t == "evl_one") {
			g = new ones(t, n, ONES);
			gates.push_back(g);
			return g;
		} else if (t == "evl_zero") {
			g = new zeros(t, n, ZEROS);
			gates.push_back(g);
			return g;
		} else if (t == "evl_input") {
			g = new input_gate(t, n, INPUT, file_name);
			gates.push_back(g);
			in_gates.push_back(((input_gate *)g));
			return g;
		} else if (t == "evl_output") {
			g = new output_gate(t, n, OUTPUT, file_name);
			gates.push_back(g);
			out_gates.push_back(((output_gate *)g));
			return g;
		} else if (t == "evl_lut") {
			g = new look_up_table(t, n, LOOK_UP_TABLE, file_name);
			gates.push_back(g);
			return g;
		} else {
			return NULL;
		}
	};
};

#endif
