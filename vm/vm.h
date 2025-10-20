//
// Created by geguj on 2025/10/12.
//

#pragma once
#include <cstdint>
#include "heap.h"
#include "op/OpCode.h"
constexpr size_t reg_num = 255;

class VirtualMachine {

    uint8_t* const_pool_top{};

    size_t pc;
    std::vector<Op>& program;
    // 返回地址栈，仅用于控制流（CALL/RET），不会作为数据栈使用
    std::vector<size_t> return_address_stack;

    std::vector<int64_t> stack;
    size_t sp;
    int64_t cmp_flag{};

    inline void execute(Op& op);
public:
    int64_t reg[reg_num]{};
    std::unique_ptr<HeapManager> heapManager;

    VirtualMachine(std::vector<Op>& program);
    ~VirtualMachine() = default;
    void run(size_t start);
    void loadProgram(const std::vector<Op>& program) const {
        this->program = program;
    }
};