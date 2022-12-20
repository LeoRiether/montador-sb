#include "lexer.hpp"

bool is_whitespace(char c) {
    return c == ' ' || c == '\r' || c == '\n';
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

struct Lexer {
private:
    Token current{ 0, 0 };
    int line{ 0 }, column{ 0 };
    LexerState st{ Idle };
    std::istream& input;
    vector<Token> buffer;

public:

    Lexer(std::istream& _input)
        : input(_input) {}

    optional<Token> next() {
        auto push_ident = [&]() {
            if (!current.empty()) {
                buffer.emplace_back(str_to_uppercase(current));
                current.clear();
            }
        };

        for (;;) {
            if (!buffer.empty()) {
                auto tok = buffer.back();
                buffer.pop_back();
                return tok;
            }

            char c = input.get();
            if (input.eof()) {
                // input.eof() só é verdadeiro
                // depois que o .get() falha
                push_ident();
                if (buffer.empty()) return {};
                else continue;
            } 

            switch (st) {
            case Idle:
                if (c == ';') st = Comment;
                else if (is_whitespace(c)) { }
                else if (c == ':') { throw "':' without label identifier"; }
                else {
                    // Start of an ident
                    current = Token{ line, column, std::string{ c } };
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
                    buffer.emplace_back(Token{ line, column, std::string{ ":" } });
                    st = Idle;
                } else {
                    current.push_back(c);
                }
                break;

            case Comment:
                if (c == '\n') st = Idle;
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
    }

    optional<Token> peek() {
        auto tok = next();
        if (tok) buffer.emplace_back(tok);
        return tok;
    }
};
