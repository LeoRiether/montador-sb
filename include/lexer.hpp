#pragma once
#include <cassert>
#include <istream>
#include <string>
#include <vector>
#include <optional>

using std::string;
using std::vector;
using std::optional;

struct Token : string {
    int line, column;

    Token(int l, int c, const string& _s = "")
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

class Lexer {
private:
    Token current{ 0, 0 };
    int line{ 0 }, column{ 0 };
    LexerState st{ Idle };
    std::istream& input;
    vector<Token> buffer;

public:

    Lexer(std::istream& _input);

    optional<Token> next();
    optional<Token> peek();
};
