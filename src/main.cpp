#include <iostream>
#include <fstream>

#include <lexer.hpp>
#include <errors.hpp>

using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
    // TODO: fazer como está especificado :)
    if (argc < 2) {
        cerr << "Especifique o arquivo em assembly a ser lido no argv[1]" << endl;
        return 1;
    }
    if (argc < 3) {
        cerr << "Especifique o arquivo de saída no argv[2]" << endl;
        return 1;
    }

    try {
        std::ifstream file(argv[1]);
        std::cout << "in" << std::endl;
        auto tokens = lex(file);
        std::cout << "out" << std::endl;
        std::cout << "???" << std::endl;

        // Print tokens
#ifdef DEBUG
        cerr << "╭ Tokens ─────────────────────────────────────────────────────────────────–––..." << endl;
        cerr << "│ ";
        for (auto tok : tokens)
            cerr << "<" << tok << "> ";
        cerr << endl;
        cerr << "╰─────────────────────────────────────────────────────────────────────────–––..." << endl;
#endif

        // auto symbols = build_symbol_table(tokens);

        // Print symbol table 
// #ifdef DEBUG
//         cerr << "╭ Symbol Table ────────────────────────────────────────────────────────────────╮" << endl;
//         for (auto [key, value] : symbols) {
//             std::string line = key + " -> " + std::to_string(value);
//             cerr << "│ " << line;
//             int padding = 77 - line.size();
//             while (padding--) cerr << ' ';
//             cerr << "│" << endl;
//         }
//         cerr << "╰──────────────────────────────────────────────────────────────────────────────╯" << endl;
// #endif

        // Assemble program...
        // auto code = assemble(tokens, symbols);

        // ...and write it to the output file
        // std::ofstream output(argv[2], std::ios::out | std::ios::binary);
        // output.write(reinterpret_cast<const char*>(&code[0]), code.size() * sizeof(code[0]));
        // output.close();
    } catch (AssemblerError& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
