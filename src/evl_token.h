#ifndef EVL_TOKEN_H
#define EVL_TOKEN_H

#include <string>

class Evl_token {
public:
	enum token_type{SINGLE, NAME, NUMBER};
private:
	token_type type;
	std::string value;
	int line;
public:
	Evl_token();
	Evl_token(token_type t, std::string s, int l);
	void set(token_type, std::string, int l);
	void set_type(token_type);
	void set_value(std::string);
	void set_line(int l);
	bool is_single();
	bool is_name();
	bool is_number();
	std::string get_value();
	int get_line();
	int get_number();
};

#endif
