#ifndef LEX_H
#define LEX_H

#include <vector>
#include "evl_token.h"

int lex_analysis(int argc, char **argv, std::vector<Evl_token> &tokens_vec);

#endif
