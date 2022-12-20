#pragma once
#include <parser.hpp>

#include <string>
#include <unordered_map>

using std::string;
using SymbolTable = std::unordered_map<string, uint16_t>;

// First pass of the codegen
SymbolTable build_symbol_table(const vector<Line> &lines);

// Second pass of the codegen
vector<uint16_t> generate_machine_code(const vector<Line> &lines,
                                       const SymbolTable &symbols);
