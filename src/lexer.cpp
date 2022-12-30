#include "lexer.hpp"

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r';
}

Token str_to_uppercase(const Token& s) {
    int n = s.size();
    Token r{ s.line, s.column };
    r.resize(n);
    for (int i = 0; i < n; i++) {
        r[i] = toupper(s[i]);
    }
    return r;
}

vector<Token> lex(std::istream& input) {
    Token current{ 0, 0 };
    int line{ 0 }, column{ 0 };
    LexerState st{ Idle };
    vector<Token> tokens;

    auto push_ident = [&]() {
        if (!current.empty()) {
            tokens.emplace_back(str_to_uppercase(current));
            current.clear();
        }
    };

    auto push_nl = [&]() {
        if (!tokens.empty() && tokens.back() != "\n")
            tokens.emplace_back(line, column, std::string{ "\n" });
    };

    for (;;) {
        char c = input.get();
        if (input.eof()) {
            // input.eof() só é verdadeiro
            // depois que o .get() falha
            break;
        } 

        switch (st) {
        case Idle:
            if (c == ';') st = Comment;
            else if (is_whitespace(c)) {}
            else if (c == '\n') { push_nl(); }
            else if (c == ':') { throw "':' without label identifier"; }
            else {
                // Start of an ident
                // TODO: Idents shouldn't begin with a digit (maybe there are
                // more specified rules about variable naming...)
                current = Token{ line, column, std::string{ c } };
                st = Ident;
            }
            break;

        case Ident:
            if (c == '\n') {
                push_ident();
                push_nl();
                st = Idle;
            } else if (c == ';') {
                push_ident();
                st = Comment;
            } else if (is_whitespace(c)) {
                push_ident();
                st = Idle;
            } else if (c == ':') {
                push_ident();
                tokens.emplace_back(line, column, std::string{ ":" });
                st = Idle;
            } else {
                current.push_back(c);
            }
            break;

        case Comment:
            if (c == '\n') {
                push_nl();
                st = Idle;
            }
            break;
        };

        // Update current position
        if (c == '\n') {
            line++;
            column = 0;
        } else {
            column++;
        }
    }

    push_ident();
    push_nl(); // tokens always end in newline, except when tokens.empty()
    return tokens;
}

std::ostream& operator<<(std::ostream& os, const vector<Token>& tokens) {
    size_t n = tokens.size();
    for (size_t i = 0; i < n; i++) {
        const auto& token = tokens[i];
        os << token;
        if (i + 1 < n && token != "\n" && tokens[i + 1] != "\n" &&
            tokens[i + 1] != ":")
            os << ' ';
    }
    return os;
}
