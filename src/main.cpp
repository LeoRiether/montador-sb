#include <box.hpp>
#include <codegen.hpp>
#include <cstring>
#include <errors.hpp>
#include <fstream>
#include <iostream>
#include <lexer.hpp>
#include <preprocessor.hpp>

using std::cerr;
using std::endl;

Box token_box(const vector<Token>& tokens, const char* title = "Tokens") {
    Box box{title};
    for (string tok : tokens) {
        if (tok != "\n") {
            box << "<" << tok << "> ";
        } else {
            box << "<\\n>\n";
        }
    }
    return box;
}

// Lê um arquivo ASM e produz um PRE
int do_preprocessing(std::string file_base) {
    std::ifstream file(file_base + ".ASM");
    std::ofstream output(file_base + ".PRE");
    if (!file.is_open()) {
        file.open(file_base + ".asm"); // abre o .asm também
                                       // pq a gente não se era .ASM ou .asm
        if (!file.is_open()) {
            cerr << "Não foi possível abrir o arquivo <" << file_base << ".ASM"
                 << "> na fase de preprocessamento de EQUs e IFs" << endl;
            return 1;
        }
    }

    // Preprocessing step 1
    vector<Token> tokens;
    try {
        tokens = lex(file);
        cerr << token_box(tokens) << endl;
        tokens = preprocess_equs_ifs(tokens);
        cerr << token_box(tokens, "Tokens (Sem EQUs e IFs)") << endl;
        output << tokens;
        output.close();
    } catch (AssemblerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Lê um arquivo PRE e produz um MCR
int do_macros(std::string file_base) {
    std::ifstream file(file_base + ".PRE");
    std::ofstream output(file_base + ".MCR");

    if (!file.is_open()) {
        cerr << "Não foi possível abrir o arquivo <" << file_base << ".ASM"
             << "> na fase de preprocessamento de EQUs e IFs" << endl;
        return 1;
    }

    // Preprocessing step 2
    vector<Token> tokens;
    try {
        tokens = lex(file);
        tokens = preprocess_macros(tokens);
        cerr << token_box(tokens, "Tokens (Sem Macros)") << endl;
        output << tokens;
        output.close();
    } catch (AssemblerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Lê um arquivo MCR e produz um OBJ
int do_object_code(std::string file_base) {
    std::ifstream file(file_base + ".MCR");
    std::ofstream output(file_base + ".OBJ");

    if (!file.is_open()) {
        cerr << "Não foi possível abrir o arquivo <" << file_base << ".ASM"
             << "> na fase de preprocessamento de EQUs e IFs" << endl;
        return 1;
    }

    try {
        // Parse tokens into lines 
        auto tokens = lex(file);
        auto lines = parse(tokens);

        // Print lines
        {
            Box box{"Lines"};
            for (const auto& line : lines)
                box << line.to_string() << "\n";
            cerr << box << endl;
        }

        auto symbols = build_symbol_table(lines);

        // Print symbol table
        {
            Box box{"Symbol Table"};
            for (const auto& [key, value] : symbols)
                box << key << " -> " << std::to_string(value) << "\n";
            cerr << box << endl;
        }

        // Assemble program...
        auto code = generate_machine_code(lines, symbols);

        // ...and write it to the output file
        bool first = true;
        for (auto x : code) {
            if (!first)
                output << ' ';
            first = false;

            output << x;
        }
        output << '\n';
        output.close();

        // Print machine code
        {
            Box box{"Machine Code"};
            for (auto x : code)
                box << std::to_string(x) << " ";
            cerr << box << endl;
        }
    } catch (AssemblerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argv[1][0] != '-') {
        cerr << "MONTADOR v1.0\n"
                "Leonardo Riether <riether.leonardo@gmail.com> e "
                "Tiago Fernandes <tiagotsf2000@gmail.com>\n"
                "\n"
                "Modo de uso: MONTADOR -<ops> <arquivo>\n"
                "    <ops>:\n"
                "        p: preprocessamento de EQUs e IFs\n"
                "        m: preprocessamento até MACROs\n"
                "        o: gera o arquivo objeto\n"
                "    <arquivo>: Nome do arquivo .ASM, sem a extensão\n"
                "\n"
                "---- OBS! ----\n"
                "É possível rodar mais de uma operação com uma única chamada "
                "do montador! Exemplo: `./MONTADOR -pmo <arquivo>`. As "
                "operações são executadas na ordem especificada, ou seja "
                "p -> m -> o"
             << endl;
        return 1;
    }

    for (char* op = argv[1] + 1; *op; op++) {
        int ret;
        if (*op == 'p') ret = do_preprocessing(argv[2]);
        else if (*op == 'm') ret = do_macros(argv[2]);
        else if (*op == 'o') ret = do_object_code(argv[2]);
        else {
            cerr << "A operação <" << argv[1] << "> não existe" << endl;
            return 1;
        }

        if (ret != 0)
            return ret;
    }

    return 0;
}
