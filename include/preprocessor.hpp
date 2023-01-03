#pragma once

#include <lexer.hpp>

#include <string>
#include <unordered_map>

using std::string;
using std::pair;
using MacroNameTable = std::unordered_map<string, pair<uint16_t, uint16_t>>;
using MacroDefinitionTable = std::vector<Token>;

vector<Token> preprocess_equs_ifs(const vector<Token>& tokens);

void macro_call(size_t &i, const vector<Token> &tokens,
                vector<Token> &processed_tokens,
                MacroNameTable &mnt, MacroDefinitionTable &mdt);
vector<Token> preprocess_macros(const vector<Token>& tokens);
