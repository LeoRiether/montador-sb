#include "montador.hpp"

size_t op_word_size(const Token& op) {
    auto is = [&](const char* s) { return op == s; };

    if (is("STOP") || is("HELLO"))
        return 1;

    if (is("ADD") || is("SUB") || is("MUL") || is("DIV") ||
        is("JMP") || is("JMPN") || is("JMPP") || is("JMPZ") ||
        is("LOAD") || is("STORE") || is("INPUT") || is("OUTPUT"))
        return 2;

    if (is("COPY"))
        return 3;

    // TODO: rething error handling decisions
    std::cerr << "Invalid op <" << op << ">" << std::endl;
    assert(false);
}

std::unordered_map<Token, uint16_t> opcode_table = {
    {"ADD",    1},
    {"SUB",    2},
    {"MUL",    3},
    {"DIV",    4},
    {"JMP",    5},
    {"JMPN",   6},
    {"JMPP",   7},
    {"JMPZ",   8},
    {"COPY",   9},
    {"LOAD",   10},
    {"STORE",  11},
    {"INPUT",  12},
    {"OUTPUT", 13},
    {"STOP",   14},
    {"HELLO",  15},
};

std::unordered_set<Token> directives = { "SPACE", "CONST" };

SymbolTable build_symbol_table(const vector<Token>& tokens) {
    SymbolTable table;
    size_t n = tokens.size();
    size_t i = 0;
    size_t pos = 0;
    while (i < n) {
        if (i+1 < n && tokens[i+1] == ":") {
            // Found a label
            table[tokens[i]] = pos;
            i += 2;
        } else if (directives.count(tokens[i])) {
            if (tokens[i] == "SPACE") {
                i++;
                pos++;
            } else if (tokens[i] == "CONST") {
                i += 2;
                pos++;
            }
        } else {
            auto words = op_word_size(tokens[i]);
            i += words;
            pos += words;
        }
    }

    return table;
}

void push_code(
    vector<uint16_t>& code, const vector<Token>& tokens,
    size_t& i, const SymbolTable& symbols
) {

    auto symbol = [&](const Token& key) {
        const auto entry = symbols.find(key);
        assert(entry != symbols.end() && "Symbol not found");
        return entry->second;
    };

    size_t n = tokens.size();

    if (directives.count(tokens[i])) {
        // token is a directive
        auto& dir = tokens[i];
        if (dir == "SPACE") {
            code.push_back(0);
            i++;
        } else if (dir == "CONST") {
            assert(i+1 < n && "missing argument for CONST");

            // CONST <label>
            if (symbols.count(tokens[i+1]))
                code.push_back(symbol(tokens[i+1]));
            // CONST <integer>
            else
                code.push_back(std::stoi(tokens[i+1]));

            i += 2;
        }

        return;
    }

    // token is an operation
    auto opcode = opcode_table[tokens[i]];
    code.push_back(opcode);

    switch (opcode) {
        case 1: case 2: case 3: case 4: // ADD, SUB, MUL, DIV
        case 5: case 6: case 7: case 8: // JMPs
        case 10: case 11: case 12: case 13: // LOAD, STORE, INPUT, OUTPUT
            // TODO: what if OP isn't a label, but is actually
            // an absolute memory position? Is this legal?
            assert(i+1 < n && "missing arguments for operation");
            code.push_back(symbol(tokens[i+1]));
            i += 2;
            break;

        case 9: // COPY
            assert(i+2 < n && "missing arguments for COPY");
            code.push_back(symbol(tokens[i+1]));
            code.push_back(symbol(tokens[i+2]));
            i += 3;
            break;

        default: // STOP, HELLO
            i++;
            break;
    }
}

vector<uint16_t> assemble(const vector<Token>& tokens, const SymbolTable& symbols) {
    vector<uint16_t> code;
    size_t n = tokens.size();
    size_t i = 0;
    while (i < n) {
        if (i+1 < n && tokens[i+1] == ":") {
            // Found a label
            i += 2;
        } else {
            // Found an operation
            push_code(code, tokens, i, symbols); 
        }
    }

    return code;
}
