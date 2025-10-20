//
// Created by geguj on 2025/10/18.
//

//
// Created by geguj on 2025/10/18.
//

#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <string>

#include "ir_token.h"

namespace ir {
    class Generator {
        size_t pos;
        size_t idx_counter{};
        std::vector<Token>& tokens;
        std::vector<uint8_t> code;

        // 符号表：标签名 -> 指令位置偏移
        std::unordered_map<std::string, size_t> symbol_table;
        // 标签引用表：需要后期填充的标签引用
        std::unordered_map<size_t, std::string> label_refs;
        // 入口点标签
        std::string entry_label;
        size_t entry_point{};

        enum class ErrLevel {
            ERROR,
            WARNING,
            NOTE
        };


    public:
        std::unordered_map<std::string, ErrLevel> errs;

        explicit Generator(std::vector<Token>& tokens);
        ~Generator();

        // 构建字节码
        bool build();
        // 获取生成的字节码
        const std::vector<uint8_t>& getCode() const { return code; }
        // 保存到文件
        bool saveToFile(const std::string& filename);

    private:
        void next_token();
        Token& cur_token() const;

        // 指令生成函数
        void gen_movri();
        void gen_movrr();
        void gen_movrm();
        void gen_movmr();
        void gen_movmi();
        void gen_movmm();

        void gen_addr();
        void gen_addm();
        void gen_addi();
        void gen_subr();
        void gen_subm();
        void gen_subi();
        void gen_mulr();
        void gen_mulm();
        void gen_muli();
        void gen_divr();
        void gen_divm();
        void gen_divi();

        void gen_call();
        void gen_ret();
        void gen_vmcall();
        void gen_jmp();
        void gen_je();
        void gen_jl();
        void gen_jg();
        void gen_jge();
        void gen_jle();
        void gen_jne();
        void gen_ble();
        void gen_bge();
        void gen_cmp();

        void gen_newi();
        void gen_newstr();
        void gen_print();

        void gen_pushr();
        void gen_popr();

        // 辅助函数
        void write_opcode(uint8_t op);
        void write_operand(const void* data, size_t size);
        void write_immediate(int64_t value);
        void write_register(uint8_t reg);
        void write_address(size_t addr);

        // 标签处理
        void process_label(const std::string& label);
        void resolve_labels();

        bool expect_token(TokenType expected, const std::string &error_msg);

        // 工具函数
        uint8_t parse_register(const std::string& reg_str);
        int64_t parse_immediate(const std::string& num_str);
    };
}