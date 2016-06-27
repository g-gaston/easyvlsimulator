#ifndef OUTPUT_GATE_H
#define OUTPUT_GATE_H

#include "gate.h"
#include "pin.h"
#include "simlib.h"
#include <fstream>
#include <iostream>

class output_gate : public gate {
	std::ofstream output_file;
public:
	output_gate(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	output_gate(std::string t, std::string n, gate_type g_t, std::string file_name) : gate(t, n, g_t) {
		output_file.open((file_name + "." + n + ".evl_output").c_str());
		if (!output_file) {
			std::cerr << "Can't read " << file_name << n << ".evl_output ." << std::endl;
			return;
		}
	};
	std::vector<int> compute_value(int width, int pos) {
		std::vector<int> value(1);
		return value;
	};
	bool is_output_pin(int pos) {
		return false;
	};
	void print_first_info() {
		output_file << itos(this->get_pin_num()) << std::endl;
		for (std::vector<pin *>::iterator pins_it = pins.begin()
				; pins_it != pins.end(); ++pins_it) {
			output_file << itos((*pins_it)->get_width()) << std::endl;
		}
	}
	int print_value() {
		std::string hex_value;
		for (std::vector<pin *>::iterator pins_it = pins.begin()
			; pins_it != pins.end(); ++pins_it) {
			try {
				hex_value = btohex((*pins_it)->get_input_value());
				output_file << hex_value << " ";
			} catch (std::exception &e) {
				std::cerr << e.what() << std::endl;
				return -1;
			}
		}
		output_file << std::endl;
		return 0;
	}

};

#endif
