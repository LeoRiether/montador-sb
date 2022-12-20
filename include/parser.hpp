#pragma once
#include <lexer.hpp>

#include <string>

using std::string;

struct Line {
    string label;
    string op;
    string arg[2];
};

vector<Line> parse(const vector<Token>& tokens);

