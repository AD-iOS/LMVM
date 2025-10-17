#include "ir_lexer.h"

std::vector<std::string> ir::Lexer::instrs = {
    "mov","add","sub","mul","div",

    "movri","movrm","movrr","movmr","movmi","movmm",
    "addr","addm","addi",
    "subr","subm","subi",
    "mulr","mulm","muli",
    "divr","divm","divi",

    "cmp","je","jl","jg","jge","jle","jne","jmp",

    "call","ret",

    "newi","newstr",

    "print"
};