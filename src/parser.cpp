#include <parser.hpp>

bool Line::operator==(const Line& rhs) const {
    if (which != rhs.which)
        return false;

    for (size_t i = 0; i < 3; i++) {
        if (data[i] != rhs.data[i])
            return false;
    }

    return true;
}

string Line::to_string() const {
    if (which == IsLabel)
        return data[0] + ":";
    if (which == IsInstruction)
        return data[0] + " " + data[1] + " " + data[2];
    if (which == IsDirective)
        return data[0] + " " + std::to_string(num);
    if (which == IsSection)
        return "SECTION " + data[0];
    return "???";
}

std::ostream& operator<<(std::ostream& os, const Line& line) {
    return os << line.to_string();
}

// TODO: talvez dê overflow pra positivos com 16 bits
// TODO: talvez esteja errado pra negativos ainda
// Faz o parsing de um número em `s`
// e retorna nullopt caso o formato seja inválido,
// ou o int16_t correspondente.
optional<int16_t> parse_number(string s) {
    int16_t ans = 0;
    bool neg = false;

    if (s[0] == '-') {
        neg = true;
        s = s.substr(1);
    }

    // The lexer already did most of the validation for us,
    // so we know that if the first character is a digit, then
    // the rest of the number should be well-formed
    if (!isdigit(s[0]))
        return {};

    // hexadecimal
    if (s.substr(0, 2) == "0X") {
        for (char c : s.substr(2)) {
            ans *= 16;
            if (c >= '0' && c <= '9')
                ans += c - '0';
            else if (c >= 'A' && c <= 'Z')
                ans += c - 'A' + 10;
        }

        return neg ? -ans : ans;
    }

    // decimal
    for (char c : s) {
        ans *= 10;
        ans += c - '0';
    }

    return neg ? -ans : ans;
}

vector<Line> parse(const vector<Token>& tokens) {
    if (tokens.empty())
        return {};

    vector<Line> lines;
    size_t n = tokens.size();
    size_t i = 0;
    while (i < n) {
        // shouldn't happen tbh
        if (tokens[i] == "\n") {
            i++;
        }

        // Parse a label
        else if (i + 1 < n && tokens[i + 1] == ":") {
            if (parse_number(tokens[i]))
                throw AssemblerError("Sintático",
                                     "Número encontrado, mas era esperado um "
                                     "identificador em <" +
                                         tokens[i] + ">",
                                     tokens[i].line, tokens[i].column);

            Line line{Line::IsLabel, {tokens[i]}};
            i += 2;

            if (i + 1 < n && tokens[i + 1] == ":") {
                throw AssemblerError(
                    "Sintático", "Duas labels foram declaradas na mesma linha",
                    tokens[i].line, tokens[i].column);
            }

            lines.emplace_back(std::move(line));
        }

        // Parse an instruction
        else if (instructions.count(tokens[i])) {
            Line line{Line::IsInstruction};
            const auto& instr = instructions.at(tokens[i]);
            line.data[0] = tokens[i];

            // Verificação dos argumentos da instrução
            for (size_t j = 1; j < instr.size; j++) {
                // Verificamos se o token i+j é válido
                if (i + j >= n)
                    throw AssemblerError(
                        "Sintático",
                        "Argumento faltando para a instrução <" + tokens[i] +
                            ">",
                        tokens[i].line, tokens[j].column);
                else if (parse_number(tokens[i]))
                    throw AssemblerError("Sintático",
                                         "Número encontrado, mas era esperado "
                                         "um identificador em <" +
                                             tokens[i] + ">",
                                         tokens[i].line, tokens[i].column);

                // Token válido! Add à linha
                line.data[j] = tokens[i + j];
            }
            i += instr.size;

            // Depois da instrução devemos ter uma nova linha
            if (i >= n || tokens[i] != "\n") {
                throw AssemblerError("Sintático",
                                     "Token extra após instrução " +
                                         tokens[i - instr.size] + ": <" +
                                         tokens[i] + ">",
                                     tokens[i].line, tokens[i].column);
            }
            i++;

            lines.emplace_back(std::move(line));
        }

        // SPACE directive
        else if (tokens[i] == "SPACE") {
            Line line{Line::IsDirective, {tokens[i]}, 1};

            // How many spaces?
            i++;
            optional<int16_t> opt;
            if (i < n && (opt = parse_number(tokens[i]))) {
                line.num = *opt;
                i++;
            }

            lines.emplace_back(std::move(line));
        }

        // CONST directive
        else if (tokens[i] == "CONST") {
            Line line{Line::IsDirective, {tokens[i]}, 1};

            // Argument
            i++;
            optional<int16_t> opt;
            if (i < n && (opt = parse_number(tokens[i]))) {
                line.num = *opt;
                i++;
            } else {
                throw AssemblerError(
                    "Sintático", "Há um argumento faltando para a diretiva CONST",
                    tokens[i - 1].line, tokens[i - 1].column);
            }

            lines.emplace_back(std::move(line));
        }

        // SECTION directive
        else if (tokens[i] == "SECTION") {
            Line line{Line::IsSection, {Token{}}};
            if (i + 1 >= n || tokens[i + 1] == "\n")
                throw AssemblerError("Sintático", "Diretiva SECTION sem argumento",
                                     tokens[i].line, tokens[i].column);
            if (tokens[i + 1] != "TEXT" && tokens[i + 1] != "DATA")
                throw AssemblerError(
                    "Sintático", "SECTION desconhecida <" + tokens[i + 1] + ">",
                    tokens[i + 1].line, tokens[i + 1].column);
            if (i + 2 < n && tokens[i + 2] != "\n")
                throw AssemblerError(
                    "Sintático",
                    "Argumento extra em declaração de seção <" + tokens[i + 2] +
                        ">",
                    tokens[i + 2].line, tokens[i + 2].column);

            line.data[0] = tokens[i + 1];
            i += 2;

            lines.emplace_back(std::move(line));
        }

        // What is this token?
        else {
            throw AssemblerError(
                "Sintático",
                "Instrução/diretiva desconhecida: <" + tokens[i] + ">",
                tokens[i].line, tokens[i].column);
        }
    }

    return lines;
}
