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
    vector<uint16_t> machine_code;
    unsigned int line_counter = 1;
    for (const auto& line : lines) {
        // Instruction
        if (line.which == Line::IsInstruction) {
            Token instruction = line.data[0];
            InstructionData instr_data = instructions.at(instruction);
            machine_code.push_back(instr_data.opcode);
            for (size_t i = 1; i < instr_data.size; i++) {
                Token argument = line.data[i];
                if (!symbols.count(argument)) {
                    throw AssemblerError(
                        "Sêmantico",
                        "O rótulo <" + argument + "> não existe.",
                        line_counter, i + 1);
                }
                uint16_t memory_address = symbols.find(argument)->second;
                machine_code.push_back(memory_address);
            }
        }
        // Directive
        else if (line.which == Line::IsDirective) {
            Token directive = line.data[0];
            if (directive == "SPACE") {
                for (int i = 0; i < line.num; i++) {
                    machine_code.push_back(0);
                }
            } else if (directive == "CONST") {
                machine_code.push_back(line.num);
            }
        }

        line_counter ++;
    }

    return machine_code;
}
