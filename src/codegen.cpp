#include <codegen.hpp>

#define UNUSED(x) (void)(x);

// First pass of the codegen
SymbolTable build_symbol_table(const vector<Line> &lines) {
    SymbolTable symbols;
    unsigned int location_counter = 0;
    bool missing_text_section = true;
    for (const auto &line : lines) {
        // Label
        if (line.which == Line::IsLabel) {
            Token label = line.data[0];
            if (symbols.count(label)) {
                throw AssemblerError(
                    "Semântico",
                    "O rótulo <" + label + "> foi definido mais de uma vez.",
                    label.line, label.column);
            }
            symbols[label] = location_counter;
        }
        // Directive
        else if (line.which == Line::IsDirective) {
            Token directive = line.data[0];
            if (directive == "SPACE") {
                location_counter += line.num;
            } else if (directive == "CONST") {
                location_counter++;
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
            if (section == "TEXT")
                missing_text_section = false;
        }
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
    for (const auto &line : lines) {
        // Instruction
        if (line.which == Line::IsInstruction) {
            Token instruction = line.data[0];
            InstructionData instr_data = instructions.at(instruction);
            machine_code.push_back(instr_data.opcode);
            for (size_t i = 1; i < instr_data.size; i++) {
                Token argument = line.data[i];
                if (!symbols.count(argument)) {
                    throw AssemblerError(
                        "Sêmantico", "O rótulo <" + argument + "> não existe.",
                        argument.line, argument.column);
                }
                // TODO: offset memory_addres by argument.num or argument.num2
                uint16_t memory_address = symbols.find(argument)->second;
                machine_code.push_back(memory_address);
            }
        }
        // Directive
        else if (line.which == Line::IsDirective) {
            Token directive = line.data[0];
            if (directive == "SPACE")
                machine_code.resize(machine_code.size() + line.num, 0);
            else if (directive == "CONST")
                machine_code.push_back(line.num);
        }
    }

    return machine_code;
}
