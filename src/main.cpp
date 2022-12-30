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
                "    Modo de uso: MONTADOR <op> <arquivo>\n"
                "        <op>:\n"
                "            -p: preprocessamento de EQUs e IFs\n"
                "            -m: preprocessamento até MACROs\n"
                "            -o: gera o arquivo objeto\n"

             << endl;
        return 1;
    }

    auto ext = file_ext(argv[1]);
    if (ext == "?") {
        cerr << "A operação <" << argv[1] << "> não existe" << endl;
        return 1;
    }

    try {
        std::ifstream file(argv[2] + std::string(".ASM"));
        std::ofstream output(argv[2] + ext);

        auto tokens = lex(file);

        // Print tokens
        cerr << "╭ Tokens "
                "──────────────────────────────────────────────────────────────"
                "───–"
                "––..."
             << endl;
        cerr << "│ ";
        for (auto tok : tokens)
            cerr << "<" << tok << "> ";
        cerr << endl;
        cerr << "╰─────────────────────────────────────────────────────────────"
                "────"
                "────────–––..."
             << endl;

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
        cerr << "╭ Lines "
                "──────────────────────────────────────────────────────────────"
                "────"
                "─────╮"
             << endl;
        for (auto x : lines) {
            std::string line = x.to_string();
            cerr << "│ " << line;
            int padding = 77 - line.size();
            while (padding--)
                cerr << ' ';
            cerr << "│" << endl;
        }
        cerr << "╰─────────────────────────────────────────────────────────────"
                "────"
                "─────────────╯"
             << endl;

        auto symbols = build_symbol_table(lines);

        // Print symbol table
        cerr << "╭ Symbol Table "
                "──────────────────────────────────────────────────────────────"
                "──╮"
             << endl;
        for (auto [key, value] : symbols) {
            std::string line = key + " -> " + std::to_string(value);
            cerr << "│ " << line;
            int padding = 77 - line.size();
            while (padding--)
                cerr << ' ';
            cerr << "│" << endl;
        }
        cerr << "╰─────────────────────────────────────────────────────────────"
                "────"
                "─────────────╯"
             << endl;

        // Assemble program...
        auto code = generate_machine_code(lines, symbols);

        // Print machine code
        cerr << "╭ Machine Code "
                "───────────────────────────────────────────────────────────–––"
                "..."
             << endl;
        cerr << "│ ";
        for (auto x : code)
            cerr << x << ' ';
        cerr << endl;
        cerr << "╰─────────────────────────────────────────────────────────────"
                "────"
                "────────–––..."
             << endl;

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
