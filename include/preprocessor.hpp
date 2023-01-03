#pragma once

#include <lexer.hpp>

#include <string>
#include <unordered_map>

using std::string;
using std::pair;
using MacroNameTable = std::unordered_map<string, pair<uint16_t, uint16_t>>;
using MacroDefinitionTable = std::vector<Token>;

vector<Token> preprocess_equs_ifs(const vector<Token>& tokens);
vector<Token> preprocess_macros(const vector<Token>& tokens);
