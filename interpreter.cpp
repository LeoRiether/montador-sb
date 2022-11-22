#include <iostream>
#include <fstream>
#include <vector>

using std::cerr;
using std::endl;
using std::vector;

enum Opcode {
    Add = 1, Sub, Mul, Div, Jmp, Jmpn, Jmpp, Jmpz, Copy,
    Load, Store, Input, Output, Stop
};

const char* opname[32] = {
    "?", "ADD", "SUB", "MUL", "DIV", "JMP", "JMPN", "JMPP",
    "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"
};

// kind of copied from assembler.cpp
// also not good code
size_t op_word_size(const uint16_t op) {
    if (op == Stop)
        return 1;

    if (op == Add || op == Sub || op == Mul || op == Div ||
        op == Jmp || op == Jmpn || op == Jmpp || op == Jmpz ||
        op == Load || op == Store || op == Input || op == Output)
        return 2;

    if (op == Copy)
        return 3;

    return 0;
}

inline int16_t do_op(int16_t a, uint16_t op, int16_t b) {
    if (op == Add) return a + b;
    if (op == Sub) return a - b;
    if (op == Mul) return a * b;
    if (op == Div) return a / b;
    return 0;
}

void interpret(vector<uint16_t>& mem) {
    mem.resize(1 << 16);
    uint16_t pc = 0;
    int16_t acc = 0;

    for (;;) {
        uint16_t op = mem[pc];
        uint16_t op1 = pc+1 < (1<<16) ? mem[pc+1] : 0;
        uint16_t op2 = pc+2 < (1<<16) ? mem[pc+2] : 0;

#ifdef DEBUG
        std::cerr << "# " << opname[op] << "(" << op << ")";
        auto ows = op_word_size(op);
        if (ows >= 2) std::cerr << " " << op1;
        if (ows >= 3) std::cerr << " " << op2;
        std::cerr << std::endl;
#endif

        switch (op) {
            case Add: case Sub: case Mul: case Div: {
                acc = do_op(acc, op, mem[op1]);
                pc += 2;
                break;
            }
            case Jmp: {
                pc = op1;
                pc += 2;
                break;
            }
            case Jmpn: {
                if (acc <  0) pc = op1;
                else pc += 2;
                break;
            }
            case Jmpp: {
                if (acc >  0) pc = op1;
                else pc += 2;
                break;
            }
            case Jmpz: {
                if (acc == 0) pc = op1;
                else pc += 2;
                break;
            }
            case Copy: {
                mem[op2] = mem[op1];
                pc += 3;
                break;
            }
            case Load: {
                acc = mem[op1];
                pc += 2;
                break;
            }
            case Store: {
                mem[op1] = acc;
                pc += 2;
                break;
            }
            case Input: {
                std::cin >> mem[op1];
                pc += 2;
                break;
            }
            case Output: {
                std::cout << "> " << mem[op1] << std::endl;
                pc += 2;
                break;
            }
            case Stop: {
                return;
            }
            default: {
                std::cerr << "Fatal error! Operation not found: " << op << std::endl;
                return;
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Especifique o arquivo binário a ser interpretado no argv[1]" << endl;
        return 1;
    }

    // Read program from input file 
    std::ifstream file(argv[1], std::ios::in | std::ios::binary);
    file.unsetf(std::ios::skipws);
    std::streampos fileSize;
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    vector<uint16_t> program(file_size / sizeof(program[0]));
    file.read((char *) &program[0], file_size);

    interpret(program);

    return 0;
}
