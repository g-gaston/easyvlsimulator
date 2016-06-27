#ifndef INPUT_GATE_H
#define INPUT_GATE_H

#include "gate.h"
#include "pin.h"
#include <fstream>
#include <iostream>
#include <sstream>

class input_gate : public gate {
	std::ifstream input_file;
	std::vector<std::vector<int> > global_input;
	int transitions;
	void read_new_line() {
		std::string line;
		if (std::getline(input_file, line) == NULL) {
			transitions = 1000;
			return;
		}
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		transitions = stoi(word);
		std::vector<int>::size_type i = 0;
		while(i < global_input.size() && iss >> word) {
			std::vector<int> aux_vec = hextob(word);
			aux_vec.resize(global_input[i].size());
			global_input[i].clear();
			global_input[i] = aux_vec;
		    i++;
		}
	}
public:
	input_gate(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	input_gate(std::string t, std::string n, gate_type g_t, std::string file_name) : gate(t, n, g_t) {
		input_file.open((file_name + "." + n + ".evl_input").c_str());
		if (!input_file) {
			std::cerr << "Can't read " << file_name << n << ".evl_input" << std::endl;
			return;
		}
		std::string line;
		std::getline(input_file, line);
		std::istringstream iss(line);
		std::string word;
		iss >> word;
		while(iss >> word) {
			std::vector<int> int_vec(stoi(word));
		    global_input.push_back(int_vec);
		}
		read_new_line();

	};
	std::vector<int> compute_value(int width, int pos) {
		// std::cout << "width: " << itos(width) << std::endl;
		// std::cout << "pos: " << itos(pos) << std::endl;
		// std::cout << "width of vec in pos: " << itos(global_input[pos].size()) << std::endl;
		// std::cout << "width of vec of vecs: " << itos(global_input.size()) << std::endl;
		if (global_input.size() <= (u_int)pos || global_input[pos].size() != (u_int)width) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), ZERO);
			return value;
		}
		//std::cout << "Returned from input: " << std::endl;
		//std::cout << std::endl;
		return global_input[pos];
	};
	bool is_output_pin(int pos) {
		return true;
	};
	void end_cycle() {
		transitions--;
		if (transitions == 0) {
			read_new_line();
		}
	}

};

#endif
