#include "lexer.hpp"

bool is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == ',';
}

vector<Token> lex(std::istream& input) {
    vector<Token> tokens;
    Token current;
    LexerState st = Idle;

    auto push_ident = [&]() {
        if (!current.empty()) {
            tokens.emplace_back(std::move(current));
            current = "";
        }
    };

    for (;;) {
        char c = input.get();
        if (input.eof()) break; // input.eof() só é verdadeiro
                                // depois que o .get() falha

        switch (st) {
        case Idle:
            if (c == ';') st = Comment;
            else if (is_whitespace(c)) { }
            else {
                // Start of an ident
                current = c;
                st = Ident;
            }
            break;

        case Ident:
            if (c == ';') {
                push_ident();
                st = Comment;
            } else if (is_whitespace(c)) {
                push_ident();
                st = Idle;
            } else if (c == ':') {
                push_ident();
                tokens.emplace_back(":");
                st = Idle;
            } else {
                current.push_back(c);
            }
            break;

        case Comment:
            if (c == '\n') st = Idle;
            break;
        };
    }

    push_ident();

    return tokens;
}
