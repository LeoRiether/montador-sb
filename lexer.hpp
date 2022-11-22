#pragma once
#include <string>
#include <vector>
#include <istream>

using Token = std::string;
using std::vector;

enum LexerState {
    Idle, Ident, Comment
};

bool is_whitespace(char);
vector<Token> lex(std::istream&);

