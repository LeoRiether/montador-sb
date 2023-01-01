#include <errors.hpp>
#include <iostream>
#include <lexer.hpp>

// similar to https://en.cppreference.com/w/cpp/string/byte/isspace
bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\f' || c == '\v';
}

bool is_ident_char(char c) {
    return isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool is_valid_hex(char c) {
    return isdigit(c) || (c >= 'A' && c <= 'Z');
}

Token str_to_uppercase(const Token& s) {
    int n = s.size();
    Token r{s.line, s.column};
    r.resize(n);
    for (int i = 0; i < n; i++) {
        r[i] = toupper(s[i]);
    }
    return r;
}

vector<Token> lex(std::istream& input) {
    Token current{0, 0};
    int line{0}, column{0};
    LexerState st{Idle};
    vector<Token> tokens;

    auto push_ident = [&]() {
        if (!current.empty()) {
            tokens.emplace_back(current);
            current.clear();
        }
    };

    auto push_nl = [&]() {
        if (!tokens.empty() && tokens.back() != "\n")
            tokens.emplace_back(line, column, std::string{"\n"});
    };

    for (;;) {
        char c = input.get();
        if (input.eof()) {
            // input.eof() só é verdadeiro
            // depois que o .get() falha
            break;
        }

        c = toupper(c);  // Case-insensitive

        switch (st) {
            case Idle:
                if (c == ';') {
                    st = Comment;
                } else if (is_whitespace(c)) {
                } else if (c == '\n') {
                    push_nl();
                } else if (c == ':') {
                    throw AssemblerError("Lexer",
                                         "':' sem identificador de label", line,
                                         column);

                } else if (isdigit(c) || c == '-') {
                    // Start of a number or hex
                    current = Token{line, column, std::string{c}};
                    st = Number;
                } else if (is_ident_char(c)) {
                    // Start of an ident
                    current = Token{line, column, std::string{c}};
                    st = Ident;
                } else {
                    throw AssemblerError(
                        "Lexer", "Caracter <" + std::string{c} + "> inesperado",
                        line, column);
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
                    tokens.emplace_back(line, column, std::string{":"});
                    st = Idle;
                } else if (is_ident_char(c) || isdigit(c)) {
                    current.push_back(c);
                } else {
                    throw AssemblerError(
                        "Lexer", "Caracter <" + std::string{c} + "> inesperado",
                        line, column);
                }
                break;

            case Number:
                if (isdigit(c)) {
                    current.push_back(c);
                } else if (c == 'X') {
                    current.push_back(c);
                    if (current != "0X" && current != "-0X")
                        throw AssemblerError(
                            "Lexer", "Número mal formado: <" + current + ">",
                            line, column);
                    st = Hex;
                } else if (c == ';') {
                    push_ident();
                    st = Comment;
                } else if (is_whitespace(c) || c == '\n') {
                    push_ident();
                    if (c == '\n')
                        push_nl();
                    st = Idle;
                } else {
                    throw AssemblerError(
                        "Lexer", "Caracter <" + std::string{c} + "> inesperado",
                        line, column);
                }
                break;

            case Hex:
                if (is_valid_hex(c)) {
                    current.push_back(c);
                } else if (is_whitespace(c) || c == '\n') {
                    push_ident();
                    if (c == '\n')
                        push_nl();
                    st = Idle;
                } else if (c == ';') {
                    push_ident();
                    st = Comment;
                } else {
                    throw AssemblerError("Lexer",
                                         "Número hexadecimal mal formado por "
                                         "causa de caracter inesperado <" +
                                             std::string{c} + "> inesperado",
                                         line, column);
                }

                break;

            case Comment:
                if (c == '\n') {
                    push_nl();
                    st = Idle;
                }
                break;
        }

        // Update current position
        if (c == '\n') {
            line++;
            column = 0;
        } else {
            column++;
        }
    }

    push_ident();
    push_nl();  // tokens always end in newline, except when tokens.empty()
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
