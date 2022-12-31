#include <codegen.hpp>

#define UNUSED(x) (void)(x);

// First pass of the codegen
SymbolTable build_symbol_table(const vector<Line> &lines) {
    UNUSED(lines);
    // TODO:
    return {};
}

// Second pass of the codegen
vector<uint16_t> generate_machine_code(const vector<Line> &lines,
                                       const SymbolTable &symbols) {
    UNUSED(lines);
    UNUSED(symbols);
    // TODO:
    return {};
}
