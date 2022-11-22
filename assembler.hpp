#pragma once
#include "lexer.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>

using SymbolTable = std::unordered_map<Token, uint16_t>;

SymbolTable build_symbol_table(const vector<Token>& tokens);
vector<uint16_t> assemble(const vector<Token>& tokens);
