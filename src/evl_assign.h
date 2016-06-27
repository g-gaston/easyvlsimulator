#ifndef EVL_ASSIGN_H
#define EVL_ASSIGN_H

#include <string>

class Evl_assign {
	std::string lhs_name;
	std::string rhs_name;
	int lhs_msb;
	int lhs_lsb;
	int rhs_msb;
	int rhs_lsb;

public:
	Evl_assign();
	void set_lhs_name(std::string n);
	void set_lhs_msb(int m);
	void set_lhs_lsb(int l);
	void set_rhs_name(std::string n);
	void set_rhs_msb(int m);
	void set_rhs_lsb(int l);
	std::string get_lhs_name();
	int get_lhs_msb();
	int get_lhs_lsb();
	std::string get_rhs_name();
	int get_rhs_msb();
	int get_rhs_lsb();
};

#endif
