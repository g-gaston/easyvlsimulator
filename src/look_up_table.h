#ifndef LOOK_UP_TABLE_H
#define LOOK_UP_TABLE_H

#include "gate.h"
#include "pin.h"
#include <limits>

class look_up_table : public gate {
	std::ifstream input_file;
	std::vector<std::vector<int> > table;
	std::ifstream& go_to_line(std::ifstream& file, unsigned long num){
	    file.seekg(std::ios::beg);
	    for(unsigned long i=0; i < num - 1; ++i){
	        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	    }
	    return file;
	}

public:
	look_up_table(std::string t, std::string n, gate_type g_t) : gate(t, n, g_t) { };
	look_up_table(std::string t, std::string n, gate_type g_t, std::string file_name) : gate(t, n, g_t) {
		input_file.open((file_name + "." + n + ".evl_lut").c_str());
		if (!input_file) {
			std::cerr << "Can't read " << file_name << n << ".evl_lut" << std::endl;
			return;
		}
		//Cachear en memoria el archivo completo ???
		std::string line;
		std::getline(input_file, line);
		while(std::getline(input_file, line) != NULL) {
		    std::vector<int> value_vec = hextob(line);
			table.push_back(value_vec);
		}
	};
	std::vector<int> compute_value(int width, int pos) {
		unsigned long dir = btoi(pins[1]->get_input_value());
		if ((unsigned long)table.size() < dir) {
			std::vector<int> value(width);
			std::fill(value.begin(), value.end(), NOT_COMPUTED);
			return value;
		}
		std::vector<int> value_vec = table[dir];
		value_vec.resize(width);
		//std::vector<int> value_vec(width);
		return value_vec;
	};
	bool is_output_pin(int pos) {
		if (pos != 0) {
			return false;
		} else {
			return true;
		}
	};

};

#endif
