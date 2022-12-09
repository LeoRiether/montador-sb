#pragma once
#include "lexer.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using SymbolTable = std::unordered_map<Token, uint16_t>;

size_t op_word_size(const Token& op);

SymbolTable build_symbol_table(const vector<Token>& tokens);

void push_code(
    vector<uint16_t>& code, const vector<Token>& tokens,
    size_t& i, SymbolTable& symbols
);

vector<uint16_t> assemble(const vector<Token>& tokens, const SymbolTable& symbols);
