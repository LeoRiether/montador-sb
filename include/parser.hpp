#pragma once
#include <errors.hpp>
#include <instruction_tables.hpp>
#include <lexer.hpp>
#include <optional>
#include <ostream>
#include <string>

using std::optional;
using std::string;

struct Line {
    enum Type { IsLabel, IsInstruction, IsDirective, IsSection };

    Type which;
    Token data[3] = {Token{}, Token{}, Token{}};
    int16_t num{0}, num2{0};  

    bool operator==(const Line& rhs) const;
    string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const Line& line);
};

bool is_identifier(const Token& tok);
optional<int16_t> parse_number(string s);

vector<Line> parse(const vector<Token>& tokens);
