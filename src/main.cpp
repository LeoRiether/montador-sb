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

std::string file_ext(const char *op) {
    if (strcmp(op, "-p") == 0)
        return ".PRE";
    if (strcmp(op, "-m") == 0)
        return ".MCR";
    if (strcmp(op, "-o") == 0)
        return ".OBJ";
    return "?";
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "MONTADOR v1.0\n"
                "Leonardo Riether <riether.leonardo@gmail.com> e "
                "Tiago Fernandes <tiagotsf2000@gmail.com>\n"
                "\n"
                "Modo de uso: MONTADOR <op> <arquivo>\n"
                "    <op>:\n"
                "        -p: preprocessamento de EQUs e IFs\n"
                "        -m: preprocessamento até MACROs\n"
                "        -o: gera o arquivo objeto\n"
                "    <arquivo>: Nome do arquivo .ASM, sem a extensão\n"
             << endl;
        return 1;
    }

    auto ext = file_ext(argv[1]);
    if (ext == "?") {
        cerr << "A operação <" << argv[1] << "> não existe" << endl;
        return 1;
    }

    std::ifstream file(argv[2] + std::string(".ASM"));
    std::ofstream output(argv[2] + ext);

    if (!file.is_open()) {
        cerr << "Não foi possível abrir o arquivo <" << argv[2] << ".ASM"
             << ">" << endl;
        return 1;
    }

    try {
        auto tokens = lex(file);

        // Print tokens
        {
            Box box{"Tokens"};
            for (const auto& tok : tokens)
                box << "<" << tok << "> ";
            cerr << box << endl;
        }

        // Preprocess EQUs and IFs
        tokens = preprocess_equs_ifs(tokens);
        if (ext == ".PRE") {
            output << tokens;
            output.close();
            return 0;
        }

        // Preprocess MACROs
        tokens = preprocess_macros(tokens);
        if (ext == ".MCR") {
            output << tokens;
            output.close();
            return 0;
        }

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

        // Print machine code
        {
            Box box{"Machine Code"};
            for (auto x : code)
                box << std::to_string(x) << " ";
            cerr << box << endl;
        }

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

    } catch (AssemblerError &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
