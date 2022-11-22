#include <iostream>
#include <fstream>

#include "lexer.hpp"
#include "assembler.hpp"

using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Especifique o arquivo em assembly a ser lido no argv[1]" << endl;
        return 1;
    }
    if (argc < 3) {
        cerr << "Especifique o arquivo de saída no argv[2]" << endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    auto tokens = lex(file);

    for (auto tok : tokens)
        cerr << "<" << tok << "> ";
    cerr << endl;

    auto words = assemble(tokens);
    std::ofstream output(argv[2], std::ios::out | std::ios::binary);
    output.write(reinterpret_cast<const char*>(&words[0]), words.size() * sizeof(words[0]));
    output.close();

    return 0;
}
