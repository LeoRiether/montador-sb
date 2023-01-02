#include <codegen.hpp>

#define UNUSED(x) (void)(x);

// First pass of the codegen
SymbolTable build_symbol_table(const vector<Line> &lines) {
    SymbolTable symbols;
    unsigned int line_counter = 1, location_counter = 0;
    bool missing_text_section = true;
    for (const auto& line : lines) {
        // Label
        if (line.which == Line::IsLabel) {
            Token label = line.data[0];
            if (symbols.count(label)) {
                throw AssemblerError(
                    "Semântico",
                    "O rótulo <" + label + "> foi definido mais de uma vez.",
                    line_counter, 0);
            }
            symbols[label] = location_counter;
        }
        // Directive
        else if (line.which == Line::IsDirective) {
            Token directive = line.data[0];
            if (directive == "SPACE") {
                location_counter += line.num;
            } else if (directive == "CONST") {
                location_counter ++;
            } else {
                throw AssemblerError("Bug: Label is neither SPACE or CONST.");
            }
        }
        // Instruction
        else if (line.which == Line::IsInstruction) {
            Token instruction = line.data[0];
            unsigned int instruction_size = instructions.at(instruction).size;
            location_counter += instruction_size;
        }
        // Section
        else if (line.which == Line::IsSection) {
            Token section = line.data[0];
            if (section == "TEXT") missing_text_section = false;
        }

        line_counter ++;
    }

    if (missing_text_section) {
        throw AssemblerError("Semântico", "Seção TEXT faltante.", 0, 0);
    }

    return symbols;
}

// Second pass of the codegen
vector<uint16_t> generate_machine_code(const vector<Line> &lines,
                                       const SymbolTable &symbols) {
    UNUSED(lines);
    UNUSED(symbols);
    // TODO:
    return {};
}
