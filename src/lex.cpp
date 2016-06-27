#include <iostream>
#include <fstream>
#include "lex.h"

int lex_analysis(int argc, char **argv, std::vector<Evl_token> &tokens_vec) {

	std::ifstream input_file(argv[1]);
	if (!input_file)
	{
		std::cerr << "I can't read " << argv[1] << "." << std::endl;
		return -1;
	}

	Evl_token token;
	std::string line;
	for (int line_no = 1; std::getline(input_file, line); ++line_no)
	{
		for (size_t i = 0; i < line.size();)
		{
			// comments
			if (line[i] == '/')
			{
				++i;
				if ((i == line.size()) || (line[i] != '/'))
				{
					std::cerr << "LINE " << line_no
						<< ": a single / is not allowed" << std::endl;
					return -1;
				}
				break; // skip the rest of the line by exiting the loop
			}

			// spaces
			if ((line[i] == ' ') || (line[i] == '\t')
				|| (line[i] == '\r') || (line[i] == '\n'))
			{
				++i; // skip this space character
				continue; // skip the rest of the iteration
			}

			// SINGLE
			if ((line[i] == '(') || (line[i] == ')')
				|| (line[i] == '[') || (line[i] == ']')
				|| (line[i] == ':') || (line[i] == ';')
				|| (line[i] == ',') || (line[i] == '='))
			{
				token.set(Evl_token::SINGLE, line.substr(i, 1), line_no);
				tokens_vec.push_back(token);
				++i; // we consumed this character
				continue; // skip the rest of the iteration
			}

			// NAME
			if (((line[i] >= 'a') && (line[i] <= 'z'))       // a to z
				|| ((line[i] >= 'A') && (line[i] <= 'Z'))    // A to Z
				|| (line[i] == '_'))
			{
				size_t name_begin = i;
				for (++i; i < line.size(); ++i)
				{
					if (!(((line[i] >= 'a') && (line[i] <= 'z'))
						|| ((line[i] >= 'A') && (line[i] <= 'Z'))
						|| ((line[i] >= '0') && (line[i] <= '9'))
						|| (line[i] == '_') || (line[i] == '$')))
					{
						break; // [name_begin, i) is the range for the token
					}
				}
				token.set(Evl_token::NAME, line.substr(name_begin, i-name_begin), line_no);
				tokens_vec.push_back(token);
				continue; // skip the rest of the operation
			}

			// NUMBER
			if ((line[i] >= '0') && (line[i] <= '9'))	// 0 to 9
			{
				size_t name_begin = i;
				for(i++; i < line.size(); i++)
				{
					if (!((line[i] >= '0') && (line[i] <= '9')))	// 0 to 9
					{
						break; // [name_begin, i) is the range for the NUMBER token
					}
				}
				token.set(Evl_token::NUMBER, line.substr(name_begin, i-name_begin), line_no);
				tokens_vec.push_back(token);
			}
			else
			{
				std::cerr << "LINE " << line_no
					<< ": invalid character" << std::endl;
				return -1;
			}
		}
	}

	return 0;
}
