#include "simlib.h"
#include <cmath>
#include <stdexcept>

std::string itos(int n) {
	std::ostringstream stm ;
	stm << n ;
	return stm.str() ;
}

int stoi(std::string n) {
	return atoi(n.c_str());
}

std::string btohex(std::vector<int> binary) {
	int bit = 0;
	int group_value = 0;
	std::stringstream sstream;
	for (std::vector<int>::iterator bin_it = binary.begin();
		bin_it != binary.end(); ++bin_it) {
        //std::cout << itos((*bin_it)) << std::endl;
		if ((*bin_it) < 0 || (*bin_it) > 1) {
			// Throw error???
            //std::cerr << "Error, trying to convert to hex not valid value" << std::endl;
            throw std::runtime_error("Error, trying to convert to hex not valid value: " + itos(*bin_it));
			break;
		}
		group_value = group_value + (int)((*bin_it)*pow(2, bit));
		bit++;
		if (bit == 4) {
			sstream << std::uppercase << std::hex << group_value;
			group_value = 0;
			bit = 0;
		}
	}
	if (bit > 0) {
	    sstream << std::uppercase << std::hex << group_value;
	}
	std::string result = sstream.str();
	std::reverse(result.begin(), result.end());
	return result;
}

std::vector<int> hexchartobin(char hex) {
    // TODO handle default / error
    std::vector<int> vec;
    switch(toupper(hex)) {
        case '0':
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(0);
        	return vec;
        case '1':
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(0);
        	return vec;
        case '2':
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(0);
        	return vec;
        case '3':
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(0);
        	return vec;
        case '4':
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(0);
        	return vec;
        case '5':
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(0);
        	return vec;
        case '6':
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(0);
        	return vec;
        case '7':
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(0);
        	return vec;
        case '8':
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(1);
        	return vec;
        case '9':
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(1);
        	return vec;
        case 'A':
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	return vec;
        case 'B':
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	return vec;
        case 'C':
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'D':
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'E':
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'F':
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'a':
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	return vec;
        case 'b':
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	return vec;
        case 'c':
        	vec.push_back(0);
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'd':
        	vec.push_back(1);
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'e':
        	vec.push_back(0);
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
        case 'f':
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	vec.push_back(1);
        	return vec;
    }
    return vec;
}

std::vector<int> hextob(std::string hex) {
	std::vector<int> v;
	for (int i = (hex.size()-1); i >= 0; i--)
	{
		std::vector<int> aux_vec = hexchartobin(hex[i]);
		v.insert(v.end(), aux_vec.begin(), aux_vec.end());
	}
	return v;
}

unsigned long btoi(std::vector<int> binary) {
	int bit = 0;
	int group_value = 0;
	for (std::vector<int>::iterator bin_it = binary.begin();
		bin_it != binary.end(); ++bin_it) {
		if ((*bin_it) < 0 || (*bin_it) > 1) {
			// Throw error???
			break;
		}
		group_value = group_value + (int)((*bin_it)*pow(2, bit));
		bit++;
	}
	return group_value;
}

