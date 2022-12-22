#pragma once
#include <unordered_map>
#include <string>

template<typename K, typename V>
using hashmap = std::unordered_map<K, V>;

using std::string;

struct InstructionData {
    uint16_t opcode;
    uint16_t size;
};

const hashmap<string, InstructionData> instructions {
    {"ADD",    { 1,  2 }},
    {"SUB",    { 2,  2 }},
    {"MUL",    { 3,  2 }},
    {"DIV",    { 4,  2 }},
    {"JMP",    { 5,  2 }},
    {"JMPN",   { 6,  2 }},
    {"JMPP",   { 7,  2 }},
    {"JMPZ",   { 8,  2 }},
    {"COPY",   { 9,  3 }},
    {"LOAD",   { 10, 2 }},
    {"STORE",  { 11, 2 }},
    {"INPUT",  { 12, 2 }},
    {"OUTPUT", { 13, 2 }},
    {"STOP",   { 14, 1 }},
};

