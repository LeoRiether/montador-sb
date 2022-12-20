#pragma once

#include <lexer.hpp>

vector<Token> preprocess_equs_ifs(const vector<Token>& tokens);
vector<Token> preprocess_macros(const vector<Token>& tokens);
