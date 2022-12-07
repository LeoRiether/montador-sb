#pragma once
#include <cassert>
#include <istream>
#include <string>
#include <vector>

using Token = std::string;
using std::vector;

enum LexerState {
    Idle, Ident, Comment
};

bool is_whitespace(char);
vector<Token> lex(std::istream&);

