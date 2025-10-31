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

VirtualMachine::VirtualMachine(std::vector<Op> &program,char** args)
    : program(program), args(args),// 初始化堆管理器
      pc(0), heapManager(std::make_unique<HeapManager>(128))
{
    memset(reg, 0, sizeof(reg));
    sp = reinterpret_cast<size_t*>(reg + reg_num);
}
#define op_data_mem(offest) (*reinterpret_cast<uint64_t*>(op.data.data() + offest))
#define op_data_imm(offest) (*reinterpret_cast< int64_t*>(op.data.data() + offest))

#define add_signed(tar) std::bit_cast< int64_t>(tar)
#define sub_signed(tar) std::bit_cast<uint64_t>(tar)

inline void VirtualMachine::execute(Op &op){

    switch (op.op) {
    case OpCode::MOVRI: {
        const auto r = op.data[0];

        reg[r] = op_data_imm(1);
        break;
    }
    case OpCode::MOVRM: {
        const auto r1 = op.data[0];

        const auto r = op.data[1];
        const auto offest = std::bit_cast<int8_t>(op.data[2]);

        if(
            const auto addr =
            std::dynamic_pointer_cast<LmInteger>(
                heapManager->loadObject(reg[r] + offest)
                )
            ){
            reg[r1] = add_signed(addr->to_ctype());
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
        const auto r = op.data[0];
        const auto offest = std::bit_cast<int8_t>(op.data[1]);

        if (
            const auto addr =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r] + offest)
                    )
        ) {
            addr->update_value(op.data.data() + 2);
        }
        break;
    }
    case OpCode::MOVMR: {
        const auto r = op.data[0]; //base register
        const auto offest = std::bit_cast<int8_t>(op.data[1]); //base + offest
        const auto r1 = op.data[2];
        if (
            const auto addr =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r] + offest)
                    )
        ) {
            addr->update_value(reg[r1]);
        }
        break;
    }
    case OpCode::MOVMM: {
        const auto r1 = op.data[0];
        const auto offest1 = std::bit_cast<int8_t>(op.data[1]);
        const auto r2 = op.data[2];
        const auto offest2 = std::bit_cast<int8_t>(op.data[3]);
        if (
            const auto addr1 =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r1] + offest1)
                        )
        ) {
            if (
                const auto addr2 =
                        std::static_pointer_cast<LmInteger>(
                            heapManager->loadObject(reg[r2] + offest2)
                        )
            ) {
                addr1->update_value(addr2->get_ptr());
            }
        }
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

        reg[r] += op_data_imm(1);
        break;
    }
    case OpCode::ADDM: {
        const auto r = op.data[0];
        const auto r1 = op.data[1]; //base register
        const auto offest1 = std::bit_cast<int8_t>(op.data[2]); //base + offest
        if (
            const auto addr1 =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r1] + offest1)
                        )
        ) {
            reg[r] += add_signed(addr1->to_ctype());
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

        reg[r] -= op_data_imm(1);
        break;
    }
    case OpCode::SUBM: {
        const auto r = op.data[0];
        const auto r1 = op.data[1]; //base register
        const auto offest1 = std::bit_cast<int8_t>(op.data[2]); //base + offest
        if (
            const auto addr1 =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r1] + offest1)
                        )
        ) {
            reg[r] -= add_signed(addr1->to_ctype());
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

        reg[r] *= op_data_imm(1);
        break;
    }
    case OpCode::MULM: {
        const auto r = op.data[0];
        const auto r1 = op.data[1]; //base register
        const auto offest1 = std::bit_cast<int8_t>(op.data[2]); //base + offest
        if (
            const auto addr1 =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r1] + offest1)
                        )
        ) {
            reg[r] *= add_signed(addr1->to_ctype());
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
        if (const auto imm = op_data_imm(1); imm != 0)
            reg[r] /= imm;
        break;
    }
    case OpCode::DIVM: {
        const auto r = op.data[0];
        const auto r1 = op.data[1]; //base register
        const auto offest1 = std::bit_cast<int8_t>(op.data[2]); //base + offest
        if (
            const auto addr1 =
                    std::static_pointer_cast<LmInteger>(
                        heapManager->loadObject(reg[r1] + offest1)
                        )
        ) {
            reg[r] /= add_signed(addr1->to_ctype());
        }
        break;
    }
    case OpCode::NEWI: { // 分配整数对象
        const auto r = op.data[0];

        reg[r] = static_cast<int64_t>(
            heapManager->storeObject(std::make_shared<LmInteger>(op_data_imm(1)))
            ); // 返回堆地址
        break;
    }
    case OpCode::NEWSTR: {
        const auto r = op.data[0];

        reg[r] = static_cast<int64_t>(
            heapManager->storeObject(std::make_shared<LmString>(reinterpret_cast<char *>(op.data.data() + 1)) )
            );
        break;
    }
    case OpCode::CALL: {

        return_address_stack.push_back(pc + 1);
        pc = op_data_mem(0) - 1; // -1 因为 run 会在 execute 后 pc++
        break;
    }
    case OpCode::RET: {
        if (!return_address_stack.empty()) {
            pc = return_address_stack.back() - 1; // -1 同上
            return_address_stack.pop_back();
        } else {
            throw LmError::format("RET: no return address at %d", pc);
        }
        break;
    }
    case OpCode::PRINT_REG: {
        const auto r = op.data[0];
        std::cout << reg[r] << '\n';
        break;
    }
    case OpCode::BLE: {
        const auto r = op.data[0];
        if (reg[r] <= 0) {
            pc = op_data_mem(1) - 1;
        }
        break;
    }
    case OpCode::BGE: {
        const auto r = op.data[0];
        if (reg[r] >= 0) {
            pc = op_data_mem(1) - 1;
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
        pc = op_data_mem(0) - 1;
        break;
    }
    case OpCode::JE: {
        if (cmp_flag == 0) {
            pc = op_data_mem(0) - 1;
        }
        break;
    }
    case OpCode::JNE: {
        if (cmp_flag != 0) {
            pc = op_data_mem(0) - 1;
        }
        break;
    }
    case OpCode::JL: {
        if (cmp_flag < 0) {
            pc = op_data_mem(0) - 1;
        }
        break;
    }
    case OpCode::JLE: {
        if (cmp_flag <= 0) {
            pc = op_data_mem(0) - 1;
        }
        break;
    }
    case OpCode::JG: {
        if (cmp_flag > 0) {
            pc = op_data_mem(0) - 1;
        }
        break;
    }
    case OpCode::JGE: {
        if (cmp_flag >= 0) {
            pc = op_data_mem(0) - 1;
        }
        break;
    }
    case OpCode::VMCALL: {
        uint16_t CallNum;
        memcpy(&CallNum, op.data.data(), sizeof(uint16_t));

        if (CallNum >= Handler::maxVMCallNum)
            throw LmError::format("VMCALL[%d]: vmcall number out of range", CallNum);


        Handler::vmcallTable[CallNum](this);
        break;
    }
    case OpCode::PUSHR: {
        const auto r = op.data[0];
        stack.push_back(add_signed(reg[r]));
        (*sp)++;
        break;
    }
    case OpCode::POPR: {
        const auto r = op.data[0];
        if (*sp == 0 || stack.empty()) {
            throw LmError::format("POPR: stack underflow at %d", pc);
        }
        reg[r] = sub_signed(stack.back());
        stack.pop_back();
        (*sp)--;
        break;
    }
    case OpCode::PAUSE: {
        vmdbg();
        break;
    }
    default: {
        throw LmError::format("Execute: unknown opcode %d", static_cast<int>(op.op));
    }
    }
}



void VirtualMachine::run(const size_t start)
{
    pc = start;
    // 使用基于范围的循环而不是索引访问以提高性能
    const size_t program_size = program.size();
    while (pc <= program_size) {
        execute(program[pc - 1]);
        pc++;
    }
}

void VirtualMachine::vmdbg() const {
    bool quit = false;
    std::string t;
    while (true) {
        std::cout << "(debug) ";
        std::getline(std::cin,t);
        switch (t[0]) {
            case 'r': {
                if (!t.substr(1).empty()) {
                    std::string count;
                    for (const auto& c : t.substr(1)) {
                        if (std::isspace(c)) continue;
                        count += c;
                    }
                    std::cout << "r" << count << " = " << reg[std::stoi(count)] << std::endl;
                }
                for (size_t i=0;i <= 15;i++) {
                    std::cout << "r" << i << " = " << reg[i] << "\n";
                }
                std::cout << "sp = " << *sp << std::endl;
                break;
            }
            case 'b': {
                quit = true;
                break;
            }
            case -1:case '\n':case '\t':case '\r':{break;}
            case 'p': {

            }
            default: {
                std::cerr << "unknown: " << t << std::endl;
                break;
            }
        }
        if (quit)return;
    }
}
