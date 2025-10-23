//
// Created by geguj on 2025/10/12.
//

#include "vm.h"
#include "call/handler.h"
#include "models/Integer.h"
#include "models/String.h"
#include "op/OpCode.h"
#include <cstdint>
#include <iostream>
#include <memory>

VirtualMachine::VirtualMachine(std::vector<Op> &program)
    : pc(0), program(program), // 初始化堆管理器
      sp(0), heapManager(std::make_unique<HeapManager>(128))
{
    memset(reg, 0, sizeof(reg));
}

void VirtualMachine::execute(Op &op)
{
    switch (op.op) {
    case OpCode::MOVRI: {
        auto r = op.data[0];
        int64_t imm;
        memcpy(&imm, op.data.data() + 1, sizeof(int64_t));

        reg[r] = imm;
        break;
    }
    case OpCode::MOVRM: {
        const auto r1 = op.data[0];
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data() + 1, sizeof(uint64_t));
        const auto obj = heapManager->loadObject(heapAddr);

        if (const auto intObj = std::dynamic_pointer_cast<LmInteger>(obj)) {
            reg[r1] = intObj->to_ctype();
        }
        break;
    }
    case OpCode::MOVRR: {
        const auto r0 = op.data[0];
        const auto r1 = op.data[1];

        reg[r0] = reg[r1];
        break;
    }

    case OpCode::MOVMI: {
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data(), sizeof(uint64_t));

        heapManager->updateObject(heapAddr, op.data.data() + 8);
        break;
    }
    case OpCode::MOVMR: {
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data(), sizeof(uint64_t));
        const auto r = op.data[8];

        heapManager->updateObject(heapAddr, &reg[r]);
        break;
    }
    case OpCode::MOVMM: {
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data(), sizeof(uint64_t));

        heapManager->updateObject(heapAddr, op.data.data() + 8);
        break;
    }
    case OpCode::ADDR: {
        const auto r0 = op.data[0];
        const auto r1 = op.data[1];

        reg[r0] += reg[r1];
        break;
    }
    case OpCode::ADDI: {
        const auto r = op.data[0];
        int64_t imm;
        memcpy(&imm, op.data.data() + 1, sizeof(int64_t));

        reg[r] += imm;
        break;
    }
    case OpCode::ADDM: {
        const auto r = op.data[0];
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data() + 1, sizeof(uint64_t));
        auto obj = heapManager->loadObject(heapAddr);

        if (auto intObj = std::dynamic_pointer_cast<LmInteger>(obj)) {
            reg[r] += intObj->to_ctype();
        }
        break;
    }
    case OpCode::SUBR: {
        const auto r0 = op.data[0];
        const auto r1 = op.data[1];

        reg[r0] -= reg[r1];
        break;
    }
    case OpCode::SUBI: {
        const auto r = op.data[0];
        int64_t imm;
        memcpy(&imm, op.data.data() + 1, sizeof(int64_t));

        reg[r] -= imm;
        break;
    }
    case OpCode::SUBM: {
        const auto r = op.data[0];
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data() + 1, sizeof(uint64_t));
        const auto obj = heapManager->loadObject(heapAddr);

        if (auto intObj = std::dynamic_pointer_cast<LmInteger>(obj)) {
            reg[r] -= intObj->to_ctype();
        }
        break;
    }
    case OpCode::MULR: {
        const auto r0 = op.data[0];
        const auto r1 = op.data[1];

        reg[r0] *= reg[r1];
        break;
    }
    case OpCode::MULI: {
        const auto r = op.data[0];

        int64_t imm;
        memcpy(&imm, op.data.data() + 1, sizeof(int64_t));
        reg[r] *= imm;
        break;
    }
    case OpCode::MULM: {
        const auto r = op.data[0];
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data() + 1, sizeof(uint64_t));
        const auto obj = heapManager->loadObject(heapAddr);
        if (auto intObj = std::dynamic_pointer_cast<LmInteger>(obj)) {
            reg[r] *= intObj->to_ctype();
        }
        break;
    }
    case OpCode::DIVR: {
        const auto r0 = op.data[0];
        const auto r1 = op.data[1];
        if (reg[r1] != 0)
            reg[r0] /= reg[r1];
        break;
    }
    case OpCode::DIVI: {
        const auto r = op.data[0];
        int64_t imm;
        memcpy(&imm, op.data.data() + 1, sizeof(int64_t));
        if (imm != 0)
            reg[r] /= imm;
        break;
    }
    case OpCode::DIVM: {
        const auto r = op.data[0];
        uint64_t heapAddr;
        memcpy(&heapAddr, op.data.data() + 1, sizeof(uint64_t));
        const auto obj = heapManager->loadObject(heapAddr);
        if (const auto intObj = std::dynamic_pointer_cast<LmInteger>(obj)) {
            if (const auto val = intObj->to_ctype(); val != 0)
                reg[r] /= val;
        }
        break;
    }
    case OpCode::NEWI: { // 分配整数对象
        const auto r = op.data[0];
        int64_t imm;
        memcpy(&imm, op.data.data() + 1, sizeof(int64_t));

        reg[r] = heapManager->storeObject(std::make_shared<LmInteger>(imm)); // 返回堆地址
        break;
    }
    case OpCode::NEWSTR: {
        const auto r = op.data[0];

        reg[r] = heapManager->storeObject(std::make_shared<LmString>(reinterpret_cast<char *>(op.data.data() + 1)));
        break;
    }
    case OpCode::CALL: {
        // op.data layout: first byte unused, next 8 bytes target (we'll read as uint64)
        uint64_t target = 0;
        memcpy(&target, op.data.data(), sizeof(uint64_t));
        // push return address on dedicated return_address_stack
        return_address_stack.push_back(pc + 1);
        pc = target - 1; // -1 因为 run 会在 execute 后 pc++
        break;
    }
    case OpCode::RET: {
        if (!return_address_stack.empty()) {
            const size_t ret = return_address_stack.back();
            return_address_stack.pop_back();
            pc = ret - 1; // -1 同上
        } else {
            throw LmError::format("RET: no return address at %d", pc);
        }
        break;
    }
    case OpCode::PRINT_REG: {
        const auto r = op.data[0];
        std::cout << int64_t(reg[r]) << '\n';
        break;
    }
    case OpCode::BLE: {
        auto r = op.data[0];
        if (reg[r] <= 0) {
            memcpy(&pc, op.data.data() + 1, sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::CMP: {
        const auto r1 = op.data[0];
        const auto r2 = op.data[1];
        cmp_flag = reg[r1] - reg[r2];
        break;
    }
    case OpCode::JMP: {
        memcpy(&pc, op.data.data(), sizeof(uint64_t));
        pc -= 1;
        break;
    }
    case OpCode::JE: {
        if (cmp_flag == 0) {
            memcpy(&pc, op.data.data(), sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::JNE: {
        if (cmp_flag != 0) {
            memcpy(&pc, op.data.data(), sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::JL: {
        if (cmp_flag < 0) {
            memcpy(&pc, op.data.data(), sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::JLE: {
        if (cmp_flag <= 0) {
            memcpy(&pc, op.data.data(), sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::JG: {
        if (cmp_flag > 0) {
            memcpy(&pc, op.data.data(), sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::JGE: {
        if (cmp_flag >= 0) {
            memcpy(&pc, op.data.data(), sizeof(uint64_t));
            pc -= 1;
        }
        break;
    }
    case OpCode::VMCALL: {
        uint16_t CallNum;
        memcpy(&CallNum, op.data.data(), sizeof(uint16_t));

        if (CallNum >= Handler::maxVMCallNum) {
            throw LmError::format("VMCALL[%d]: vmcall number out of range", CallNum);
        }

        Handler::vmcallTable[CallNum](this);
        break;
    }
    case OpCode::PUSHR: {
        const auto r = op.data[0];
        stack.push_back(reg[r]);
        sp++;
        break;
    }
    case OpCode::POPR: {
        const auto r = op.data[0];
        if (sp == 0 || stack.empty()) {
            throw LmError::format("POPR: stack underflow at %d", pc);
        }
        reg[r] = stack.back();
        stack.pop_back();
        sp--;
        break;
    }
    default: {
        throw LmError::format("Execute: unknown opcode %d", static_cast<int>(op.op));
    }
    }
}

#include <string>

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

void VirtualMachine::run(const size_t start)
{
    pc = start;
    while (pc <= program.size()) {
        execute(program[pc - 1]);
        //if (pc > program.size()) {
            // pc超出范围，段错误
        //    LmError::format("Segmentation fault at %d", pc);
        //}
        pc++;
    }
}
