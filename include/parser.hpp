#pragma once
#include <errors.hpp>
#include <instruction_tables.hpp>
#include <lexer.hpp>
#include <optional>
#include <string>
#include <ostream>

using std::string;
using std::optional;

struct Line {
    enum Type { IsLabel, IsInstruction, IsDirective, IsSection };

    Type which;
    Token data[3] = { Token{}, Token{}, Token{} };
    int16_t num{0}; // only used if which == IsDirective...
                    // because I don't know how to work with unions
                    // in C++!!!
                    // thanks Bjarne

    bool operator==(const Line& rhs) const;
    string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const Line& line);
};

bool valid_identifier(const string& s);
optional<int16_t> parse_number(string s);

vector<Line> parse(const vector<Token>& tokens);
