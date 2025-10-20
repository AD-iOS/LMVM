//
// Created by geguj on 2025/10/12.
//

#pragma once
#include <vector>
#include <cstdint>

enum class OpCode {
    MOVRR,MOVRI,MOVRM,MOVMR,MOVMI,MOVMM,
    NEWI,NEWSTR,
    ADDR,ADDM,ADDI,
    SUBR,SUBM,SUBI,
    MULR,MULI,MULM,
    DIVR,DIVI,DIVM,

    CALL, RET, VMCALL,

    PRINT_REG,

    CMP,JMP,
    JE, // ==
    JNE,// !=
    JG, // >
    JGE,// >=
    JL, // <
    JLE,// <=

    BLE,BGE,
    PUSHR,POPR,
};
struct Op {
    OpCode op;
    alignas(8) std::vector<uint8_t> data;
};
