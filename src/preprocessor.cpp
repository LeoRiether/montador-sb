#include <preprocessor.hpp>
#include <parser.hpp>

using std::string;

vector<Token> preprocess_equs_ifs(const vector<Token>& tokens) {
    // TODO:
    return tokens;
}

void macro_call(size_t &i, const vector<Token> &tokens,
                vector<Token> &processed_tokens,
                MacroNameTable &mnt, MacroDefinitionTable &mdt) {

    Token macro = tokens[i];
    auto [number_arguments, definition_address] = mnt[macro];
    std::unordered_map<string, Token> arguments;

    // Process arguments
    i++;
    while (tokens[i] != "\n") {
        string arg_label = "#" + to_string(arguments.size() + 1);
        arguments[arg_label] = tokens[i];
        i++;
    }

    if (arguments.size() != number_arguments) {
        throw AssemblerError(
            "Semântico",
            "Quantidade de argumentos da chamada de macro está incorreta.",
            macro.line, macro.column-1);
    }

    unsigned j = definition_address;
    while (mdt[j] != "ENDMACRO") {
        if (arguments.count(mdt[j])) {
            processed_tokens.push_back(Token(
                mdt[j].line, mdt[j].column, arguments[mdt[j]])
            );
        } else {
            processed_tokens.push_back(mdt[j]);
        }

        j ++;
    }
}

vector<Token> preprocess_macros(const vector<Token>& tokens) {
    vector<Token> processed_tokens;
    MacroNameTable mnt;
    MacroDefinitionTable mdt;

    size_t n = tokens.size();

    for (size_t i = 0; i < n; i++) {

        // Macro definition
        if (tokens[i] == "MACRO") {
            Token macro = tokens[i-2];

            if (i < 2 || !is_identifier(macro) || tokens[i-1] != ":") {
                throw AssemblerError(
                    "Sintático",
                    "Sintaxe da macro está incorreta.",
                    tokens[i].line, tokens[i].column);
            }
            if (mnt.count(macro)) {
                throw AssemblerError(
                    "Semântico",
                    "A macro <" + macro +
                    "> foi definida mais de uma vez.",
                    macro.line, macro.column);
            }
            if (mnt.size() == 2) {
                throw AssemblerError(
                    "Semântico",
                    "Não é possível definir mais de duas macros",
                    macro.line, macro.column
                );
            }

            processed_tokens.pop_back(); // :
            processed_tokens.pop_back(); // macro label

            std::unordered_map<string, int> arguments;

            // Process arguments
            i ++;
            while (tokens[i] != "\n") {
                if (tokens[i][0] != '&') {
                    throw AssemblerError(
                        "Sintático",
                        "Sintaxe dos argumentos da macro está incorreta",
                        tokens[i].line, tokens[i].column);
                }

                arguments[tokens[i]] = arguments.size() + 1;
                i ++;
            }

            if (arguments.size() > 3) {
                throw AssemblerError(
                    "Semântico",
                    "Não é possível utilizar mais de 3 argumentos na macro",
                    macro.line, macro.column
                );
            }

            mnt[macro] = {arguments.size(), mdt.size()};

            i ++;
            while (tokens[i] != "ENDMACRO") {
                // Argument
                if (arguments.count(tokens[i])) {
                    string arg_label = "#" + to_string(arguments[tokens[i]]);
                    mdt.push_back(Token(
                        tokens[i].line, tokens[i].column, arg_label)
                    );
                }
                // Macro call inside the macro definition
                else if (mnt.count(tokens[i])) {
                    macro_call(i, tokens, mdt, mnt, mdt);
                }
                else{
                    mdt.push_back(tokens[i]);
                }

                i ++;
                if (i == n) {
                    throw AssemblerError(
                        "Sintático",
                        "ENDMACRO não encontrado",
                        macro.line, macro.column);
                }
            }
            mdt.push_back(tokens[i]); // ENDMACRO
            i++;
        }

        // Macro call
        else if (mnt.count(tokens[i])) {
            macro_call(i, tokens, processed_tokens, mnt, mdt);
        }

        // Not related to macros
        else {
            processed_tokens.push_back(tokens[i]);
        }
    }

    return processed_tokens;
}
