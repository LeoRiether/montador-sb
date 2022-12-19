#pragma once
#include <cassert>
#include <istream>
#include <string>
#include <vector>
#include <optional>

using std::string;
using std::optional;
using std::vector;

struct Token : string {
    int line, column;

    Token(int l, int c, const string& _s = "")
        : string(_s), line(l), column(c) {}

    void set_position(int l, int c) {
        line = l;
        column = c;
    }
};


enum LexerState {
    Idle, Ident, Comment
};

bool is_whitespace(char);
vector<Token> lex(std::istream&);

