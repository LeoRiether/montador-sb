#include <codegen.hpp>
#include <errors.hpp>
#include <fstream>
#include <iostream>
#include <lexer.hpp>
#include <preprocessor.hpp>

using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
    // TODO: fazer como está especificado :)
    if (argc < 2) {
        cerr << "Especifique o arquivo em assembly a ser lido no argv[1]"
             << endl;
        return 1;
    }
    if (argc < 3) {
        cerr << "Especifique o arquivo de saída no argv[2]" << endl;
        return 1;
    }

    try {
        std::ifstream file(argv[1]);
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

        tokens = preprocess_equs_ifs(tokens);
        tokens = preprocess_macros(tokens);

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
        std::ofstream output(argv[2], std::ios::out);
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
