#ifndef SIMLIB_H
#define SIMLIB_H

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

#define ZERO 			 0
#define ONE	 			 1
#define Z			 	-1
#define NOT_COMPUTED	-2

std::string itos(int n);
int stoi(std::string n);
std::string btohex(std::vector<int> binary);
std::vector<int> hextob(std::string hex);
unsigned long btoi(std::vector<int> binary);

#endif
