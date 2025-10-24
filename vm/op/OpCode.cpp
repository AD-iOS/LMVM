//
// Created by geguj on 2025/10/12.
//

#include "OpCode.h"


std::string OpCode2Str(OpCode op)
{
    switch (op) {
    case OpCode::MOVRR:
        return "MOVRR";
    case OpCode::MOVRI:
        return "MOVRI";
    case OpCode::MOVRM:
        return "MOVRM";
    case OpCode::MOVMR:
        return "MOVMR";
    case OpCode::MOVMI:
        return "MOVMI";
    case OpCode::MOVMM:
        return "MOVMM";
    case OpCode::NEWI:
        return "NEWI";
    case OpCode::NEWSTR:
        return "NEWSTR";
    case OpCode::ADDR:
        return "ADDR";
    case OpCode::ADDM:
        return "ADDM";
    case OpCode::ADDI:
        return "ADDI";
    case OpCode::SUBR:
        return "SUBR";
    case OpCode::SUBM:
        return "SUBM";
    case OpCode::SUBI:
        return "SUBI";
    case OpCode::MULR:
        return "MULR";
    case OpCode::MULI:
        return "MULI";
    case OpCode::MULM:
        return "MULM";
    case OpCode::DIVR:
        return "DIVR";
    case OpCode::DIVI:
        return "DIVI";
    case OpCode::DIVM:
        return "DIVM";
    case OpCode::CALL:
        return "CALL";
    case OpCode::RET:
        return "RET";
    case OpCode::VMCALL:
        return "VMCALL";
    case OpCode::PRINT_REG:
        return "PRINT_REG";
    case OpCode::CMP:
        return "CMP";
    case OpCode::JMP:
        return "JMP";
    case OpCode::JE:
        return "JE";
    case OpCode::JNE:
        return "JNE";
    case OpCode::JG:
        return "JG";
    case OpCode::JGE:
        return "JGE";
    case OpCode::JL:
        return "JL";
    case OpCode::JLE:
        return "JLE";
    case OpCode::BLE:
        return "BLE";
    case OpCode::BGE:
        return "BGE";
    case OpCode::PUSHR:
        return "PUSHR";
    case OpCode::POPR:
        return "POPR";
    default:
        return "UNKNOWN";
    }
}