//
// Created by geguj on 2025/10/18.
//

#include "ir_generator.h"
#include "../vm/op/OpCode.h"
#include <fstream>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <string>
#include <algorithm>
#include <cctype>




namespace ir {
    void trim(std::string &str) {
        auto is_space = [](unsigned char c) { return std::isspace(c); };

        const auto left = std::find_if_not(str.begin(), str.end(), is_space);
        const auto right = std::find_if_not(str.rbegin(), str.rend(), is_space);

        str.erase(right.base(), str.end());
        str.erase(str.begin(), left);
    }


    Generator::Generator(std::vector<Token> &tokens) : pos(0), tokens(tokens) {
    }

    Generator::~Generator() = default;

    void Generator::next_token() {
        if (pos < tokens.size() - 1)
            pos++;
    }

    Token &Generator::cur_token() const {
        return tokens[pos];
    }

    bool Generator::build() {
        // 第一遍：收集标签定义
        pos = 0;

        while (pos < tokens.size()) {
            Token &token = tokens[pos];

            if (token.type == TokenType::IDENT && pos + 1 < tokens.size() &&
                tokens[pos + 1].type == TokenType::COLON) {
                // 标签定义
                process_label(token.value);
                pos += 2; // 跳过标签名和冒号
            } else if (token.type == TokenType::MACRO && token.value == "entry") {
                // 入口点定义
                pos++; // 跳过entry
                if (pos < tokens.size() && tokens[pos].type == TokenType::IDENT) {
                    entry_label = tokens[pos].value;
                }
                pos++; // 跳过标签名
            } else {
                pos++; // 移动到下一个token
            }
        }

        // 第二遍：生成代码
        pos = 0;
        while (pos < tokens.size()) {
            Token &token = tokens[pos];
            trim(token.value);
            if (token.type == TokenType::OPCODE) {
                // 根据操作码调用相应的生成函数
                if (token.value == "mov") gen_movrr(); // mov 别名
                else if (token.value == "movri") gen_movri();
                else if (token.value == "movrr") gen_movrr();
                else if (token.value == "movrm") gen_movrm();
                else if (token.value == "movmr") gen_movmr();
                else if (token.value == "movmi") gen_movmi();
                else if (token.value == "movmm") gen_movmm();
                else if (token.value == "add") gen_addr(); // add 别名
                else if (token.value == "addr") gen_addr();
                else if (token.value == "addm") gen_addm();
                else if (token.value == "addi") gen_addi();
                else if (token.value == "sub") gen_subr(); // sub 别名
                else if (token.value == "subr") gen_subr();
                else if (token.value == "subm") gen_subm();
                else if (token.value == "subi") gen_subi();
                else if (token.value == "mul") gen_mulr(); // mul 别名
                else if (token.value == "mulr") gen_mulr();
                else if (token.value == "mulm") gen_mulm();
                else if (token.value == "muli") gen_muli();
                else if (token.value == "div") gen_divr(); // div 别名
                else if (token.value == "divr") gen_divr();
                else if (token.value == "divm") gen_divm();
                else if (token.value == "divi") gen_divi();
                else if (token.value == "call") gen_call();
                else if (token.value == "ret") gen_ret();
                else if (token.value == "vmcall") gen_vmcall();
                else if (token.value == "jmp") gen_jmp();
                else if (token.value == "je") gen_je();
                else if (token.value == "jl") gen_jl();
                else if (token.value == "jg") gen_jg();
                else if (token.value == "jge") gen_jge();
                else if (token.value == "jle") gen_jle();
                else if (token.value == "jne") gen_jne();
                else if (token.value == "ble") gen_ble();
                else if (token.value == "bge") gen_bge();
                else if (token.value == "cmp") gen_cmp();
                else if (token.value == "newi") gen_newi();
                else if (token.value == "newstr") gen_newstr();
                else if (token.value == "print") gen_print();
                else {
                    errs["Unknown opcode: " + token.value] = ErrLevel::ERROR;
                    pos++; // 确保前进
                }
            } else if (token.type == TokenType::IDENT && pos + 1 < tokens.size() &&
                       tokens[pos + 1].type == TokenType::COLON) {
                // 跳过标签定义
                pos += 2; // 跳过标签名和冒号
            } else if (token.type == TokenType::MACRO && token.value == "entry") {
                // 跳过entry宏
                pos++; // 跳过entry
                if (pos < tokens.size() && tokens[pos].type == TokenType::IDENT) {
                    pos++; // 跳过标签名
                }
            } else if (token.type == TokenType::COMMENT) {
                // 跳过注释
                pos++;
            } else {
                // 其他token，跳过
                pos++;
            }
        }

        // 解析标签引用
        resolve_labels();

        return errs.empty();
    }

    // 辅助函数实现
    void Generator::write_opcode(uint8_t op) {
        code.push_back(op);
    }

    void Generator::write_operand(const void *data, size_t size) {
        const uint8_t *bytes = static_cast<const uint8_t *>(data);
        code.insert(code.end(), bytes, bytes + size);
    }

    void Generator::write_immediate(int64_t value) {
        write_operand(&value, sizeof(value));
    }

    void Generator::write_register(uint8_t reg) {
        code.push_back(reg);
    }

    void Generator::write_address(size_t addr) {
        write_operand(&addr, sizeof(addr));
    }

    uint8_t Generator::parse_register(const std::string &reg_str) {
        if (reg_str == "sp") return 255; // SP寄存器
        if (reg_str.length() > 1 && reg_str[0] == 'r') {
            return static_cast<uint8_t>(std::stoi(reg_str.substr(1)));
        }
        return 0;
    }

    int64_t Generator::parse_immediate(const std::string &num_str) {
        return std::stoll(num_str);
    }

    void Generator::process_label(const std::string &label) {
        // 当前代码位置就是标签的指令偏移
        symbol_table[label] = code.size();
    }

    void Generator::resolve_labels() {
        // 设置入口点
        if (!entry_label.empty() && symbol_table.count(entry_label)) {
            entry_point = symbol_table[entry_label];
        }

        // 解析所有标签引用
        for (const auto &ref: label_refs) {
            size_t code_pos = ref.first;
            const std::string &label = ref.second;

            if (symbol_table.count(label)) {
                size_t target_addr = symbol_table[label];
                // 替换标签引用为实际地址
                std::memcpy(code.data() + code_pos, &target_addr, sizeof(target_addr));
            } else {
                errs["Undefined label: " + label] = ErrLevel::ERROR;
            }
        }
    }

    // 通用错误检查函数
    bool Generator::expect_token(TokenType expected, const std::string &error_msg) {
        if (cur_token().type != expected) {
            errs[error_msg + " at line " + std::to_string(cur_token().line) +
                 " column " + std::to_string(cur_token().column)] = ErrLevel::ERROR;
            return false;
        }
        return true;
    }

    // 指令生成函数实现

    // MOV 指令系列
    void Generator::gen_movri() {
        write_opcode(static_cast<uint8_t>(OpCode::MOVRI));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节立即数

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::NUM, "Expected number")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    void Generator::gen_movrr() {
        write_opcode(static_cast<uint8_t>(OpCode::MOVRR));
        code.push_back(2); // 长度: 2字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_movrm() {
        write_opcode(static_cast<uint8_t>(OpCode::MOVRM));
        code.push_back(9); // 长度: 1字节寄存器+8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
    }

    void Generator::gen_movmr() {
        write_opcode(static_cast<uint8_t>(OpCode::MOVMR));
        code.push_back(9); // 长度: 8字节地址+1字节寄存器

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_movmi() {
        write_opcode(static_cast<uint8_t>(OpCode::MOVMI));
        code.push_back(16); // 长度: 8字节地址 + 8字节立即数

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::NUM, "Expected number")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    void Generator::gen_movmm() {
        write_opcode(static_cast<uint8_t>(OpCode::MOVMM));
        code.push_back(16); // 长度: 8字节源地址 + 8字节目标地址

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 源地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for source address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 目标地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for destination address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
    }

    // ADD 指令系列
    void Generator::gen_addr() {
        write_opcode(static_cast<uint8_t>(OpCode::ADDR));
        code.push_back(2); // 长度: 2字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_addm() {
        write_opcode(static_cast<uint8_t>(OpCode::ADDM));
        code.push_back(9); // 长度: 1字节寄存器+8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
    }

    void Generator::gen_addi() {
        write_opcode(static_cast<uint8_t>(OpCode::ADDI));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节立即数

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::NUM, "Expected number")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    // SUB 指令系列
    void Generator::gen_subr() {
        write_opcode(static_cast<uint8_t>(OpCode::SUBR));
        code.push_back(2); // 长度: 2字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_subm() {
        write_opcode(static_cast<uint8_t>(OpCode::SUBM));
        code.push_back(9); // 长度: 1字节寄存器+8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
    }

    void Generator::gen_subi() {
        write_opcode(static_cast<uint8_t>(OpCode::SUBI));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节立即数

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::NUM, "Expected number")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    // MUL 指令系列
    void Generator::gen_mulr() {
        write_opcode(static_cast<uint8_t>(OpCode::MULR));
        code.push_back(2); // 长度: 2字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_mulm() {
        write_opcode(static_cast<uint8_t>(OpCode::MULM));
        code.push_back(9); // 长度: 1字节寄存器+8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        //立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
    }

    void Generator::gen_muli() {
        write_opcode(static_cast<uint8_t>(OpCode::MULI));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节立即数

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::NUM, "Expected number")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    // DIV 指令系列
    void Generator::gen_divr() {
        write_opcode(static_cast<uint8_t>(OpCode::DIVR));
        code.push_back(2); // 长度: 2字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_divm() {
        write_opcode(static_cast<uint8_t>(OpCode::DIVM));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::LEFT_BRACKET, "Expected '['")) return;

        next_token();
        // 立即数地址
        if (cur_token().type == TokenType::NUM) {
            write_immediate(parse_immediate(cur_token().value));
        } else {
            errs["Expected register or number for address"] = ErrLevel::ERROR;
            return;
        }

        next_token();
        if (!expect_token(TokenType::RIGHT_BRACKET, "Expected ']'")) return;

        next_token();
    }

    void Generator::gen_divi() {
        write_opcode(static_cast<uint8_t>(OpCode::DIVI));
        code.push_back(9); // 长度: 1字节寄存器+8字节立即数

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::NUM, "Expected number")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    // 控制流指令
    void Generator::gen_call() {
        write_opcode(static_cast<uint8_t>(OpCode::CALL));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0); // 先写0，稍后替换
        } else {
            errs["Expected label for call"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_ret() {
        write_opcode(static_cast<uint8_t>(OpCode::RET));
        code.push_back(0); // 无操作数
        next_token();
    }

    void Generator::gen_vmcall() {
        write_opcode(static_cast<uint8_t>(OpCode::VMCALL));
        code.push_back(2); // 长度: 2字节调用号

        next_token();
        if (!expect_token(TokenType::NUM, "Expected VM call number")) return;

        auto call_num = static_cast<uint16_t>(parse_immediate(cur_token().value));
        write_operand(&call_num, sizeof(call_num));

        next_token();
    }

    void Generator::gen_jmp() {
        write_opcode(static_cast<uint8_t>(OpCode::JMP));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for jmp"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_je() {
        write_opcode(static_cast<uint8_t>(OpCode::JE));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for je"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_jl() {
        write_opcode(static_cast<uint8_t>(OpCode::JL));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for jl"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_jg() {
        write_opcode(static_cast<uint8_t>(OpCode::JG));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for jg"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_jge() {
        write_opcode(static_cast<uint8_t>(OpCode::JGE));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for jge"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_jle() {
        write_opcode(static_cast<uint8_t>(OpCode::JLE));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for jle"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_jne() {
        write_opcode(static_cast<uint8_t>(OpCode::JNE));
        code.push_back(8); // 长度: 8字节地址

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for jne"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_ble() {
        write_opcode(static_cast<uint8_t>(OpCode::BLE));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for ble"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_bge() {
        write_opcode(static_cast<uint8_t>(OpCode::BGE));
        code.push_back(9); // 长度: 1字节寄存器 + 8字节地址

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (cur_token().type == TokenType::IDENT) {
            label_refs[code.size()] = cur_token().value;
            write_address(0);
        } else {
            errs["Expected label for bge"] = ErrLevel::ERROR;
        }

        next_token();
    }

    void Generator::gen_cmp() {
        write_opcode(static_cast<uint8_t>(OpCode::CMP));
        code.push_back(2); // 长度: 2字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
        if (!expect_token(TokenType::COMMA, "Expected comma")) return;

        next_token();
        if (!expect_token(TokenType::REG, "Expected register")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    void Generator::gen_newi() {
        write_opcode(static_cast<uint8_t>(OpCode::NEWI));
        code.push_back(8); // 长度: 8字节大小

        next_token();
        if (!expect_token(TokenType::NUM, "Expected allocation size")) return;
        write_immediate(parse_immediate(cur_token().value));

        next_token();
    }

    void Generator::gen_newstr() {
        write_opcode(static_cast<uint8_t>(OpCode::NEWSTR));

        next_token();
        if (!expect_token(TokenType::REG, "Expected register for string address")) return;
        uint8_t reg = parse_register(cur_token().value);

        next_token();
        // 字符串字面量处理
        if (cur_token().type != TokenType::STR) {
            errs["Expected string literal"] = ErrLevel::ERROR;
            return;
        }

        std::string str_literal = cur_token().value;
        // 处理转义序列
        std::string processed_str;
        for (size_t i = 0; i < str_literal.length(); ++i) {
            if (str_literal[i] == '\\' && i + 1 < str_literal.length()) {
                switch (str_literal[i + 1]) {
                    case 'n': processed_str += '\n';
                        break;
                    case 't': processed_str += '\t';
                        break;
                    case 'r': processed_str += '\r';
                        break;
                    case '0': processed_str += '\0';
                        break;
                    case '\\': processed_str += '\\';
                        break;
                    default: processed_str += str_literal[i + 1];
                        break;
                }
                i++; // 跳过转义字符
            } else {
                processed_str += str_literal[i];
            }
        }

        // 操作数长度: 1字节寄存器 + 字符串长度 + 1字节终止符
        code.push_back(1 + processed_str.length());
        write_register(reg);
        write_operand(processed_str.c_str(), processed_str.length()); // 包含null终止符

        next_token();
    }

    void Generator::gen_print() {
        write_opcode(static_cast<uint8_t>(OpCode::PRINT_REG));
        code.push_back(1); // 长度: 1字节寄存器

        next_token();
        if (!expect_token(TokenType::REG, "Expected register to print")) return;
        write_register(parse_register(cur_token().value));

        next_token();
    }

    bool Generator::saveToFile(const std::string &filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file :" << filename << '\n';
            return false;
        }

        // 写入魔数 "QTLM"
        const char magic[] = {'Q', 'T', 'L', 'M'};
        file.write(magic, 4);

        // 写入版本 (当前版本1)
        uint32_t version = 1;
        file.write(reinterpret_cast<const char *>(&version), 4);

        // 写入入口点
        file.write(reinterpret_cast<const char *>(&entry_point), 8);

        // 写入指令代码
        file.write(reinterpret_cast<const char *>(code.data()), code.size());

        file.close();
        return true;
    }
}
