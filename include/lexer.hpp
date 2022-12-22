#pragma once
#include <cassert>
#include <istream>
#include <string>
#include <vector>
#include <optional>

using std::string;
using std::vector;

struct Token : string {
    int line, column;

    Token(int l = 0, int c = 0, const string& _s = "")
        : string(_s), line(l), column(c) {}

    inline void set_position(int l, int c) {
        line = l;
        column = c;
    }
};

bool is_whitespace(char);
Token str_to_uppercase(const Token& s);

enum LexerState {
    Idle, Ident, Comment
};

vector<Token> lex(std::istream& input);
