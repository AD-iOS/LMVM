#include "ir_token.h"

#include <algorithm>
#include <cctype>
#include <iostream>

namespace ir {
    void lexer_error(const size_t line,const size_t col,const std::string &e,const std::string &f) {
        const std::string format = "Lmir Error(" +
                std::to_string(line) + ": " +
                std::to_string(col) + "): `\033[31m" + e + "\033[0m` " + f + '\n';
        throw std::runtime_error(format.c_str());
    }
    std::vector<std::string> Lexer::ops = {
        "mov","add","sub","mul","div",

        "movri","movrm","movrr","movmr","movmi","movmm",
        "addr","addm","addi",
        "subr","subm","subi",
        "mulr","mulm","muli",
        "divr","divm","divi",

        "cmp","je","jl","jg","jge","jle","jne","jmp",

        "call","ret",

        "newi","newstr",

        "print",
        "vmcall"
    };
    std::vector<std::string> Lexer::macros = {
        "entry"
    };




// Token转为字符串(DEBUG)
std::string Token::toString() const {
    switch(type) {
        case TokenType::OPCODE: return "OPCODE";
        case TokenType::REG: return "REGISTER";
        case TokenType::IDENT: return "IDENTIFIER";
        case TokenType::NUM: return "NUMBER";
        case TokenType::COMMENT: return "COMMENT";
        case TokenType::END: return "END_OF_FILE";
        case TokenType::UNKNOWN: return "UNKNOWN";
        case TokenType::COMMA: return "COMMA";
        case TokenType::COLON: return "COLON";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MUL: return "MUL";
        case TokenType::DIV: return "DIV";
        case TokenType::SP_REG: return "SP_REG";
        case TokenType::MACRO: return "MACRO";
        case TokenType::STR: return "STR";
        default: return "UNKNOWN";
    }
}


Lexer::Lexer(std::string& source)
    : source(source), pos(0), length(source.length()), currentLine(1), currentColumn(1) {
    std::ranges::sort(ops);
}

// 获取当前字符
char Lexer::currentChar()const {
    return (pos < length) ? source[pos] : '\0';
}

// 前进到下一个字符，并更新行列号信息
void Lexer::nextChar() {
    if (pos < length) {
        currentColumn++;
        pos++;
    }
}

// 跳过空白字符
void Lexer::skipWhitespace() {
    while (pos < length && std::isspace(currentChar())) {
        if (currentChar() == '\n') {
            currentLine++;
            currentColumn = 0;
        }
        pos++;
        currentColumn = (currentChar() == '\n') ? 0 : currentColumn + 1;
    }
}

// 核心函数：获取下一个Token
Token Lexer::getNextToken() {
    // 跳过所有空白字符
    skipWhitespace();

    // 检查是否已到达源代码末尾
    if (pos >= length) {
        return Token(TokenType::END, "", currentLine, currentColumn);
    }

    // 记录当前Token开始的位置
    const auto tokenLine = currentLine;
    const auto tokenColumn = currentColumn;
    const char ch = currentChar();

    // 识别注释
    if (ch == '#') {
        std::string comment;
        while (pos < length && currentChar() != '\n') {
            comment += currentChar();
            nextChar();
        }
        return Token(TokenType::COMMENT, comment, tokenLine, tokenColumn);
    }

    //字符串
    if (ch == '"') {
        std::string comment;
        while (true) {
            nextChar();
            if (currentChar() == '"') {
                nextChar();
                break;
            }
            if (currentChar() == EOF || currentChar() == '\n') {
                std::cerr << "Error: Unclosed string literal at "
                     << tokenLine << ":" << tokenColumn << " `" << comment << "`\n";
                break;
            }
            comment += currentChar();
        }

        return Token(TokenType::STR, comment, tokenLine, tokenColumn);
    }

    // 识别数字（仅十进制）
    if (std::isdigit(ch)) {
        std::string value;
        while (pos < length && std::isdigit(currentChar()) ) {
            value += currentChar();
            nextChar();
        }
        return Token(TokenType::NUM, value, tokenLine, tokenColumn);
    }

    // 识别单个字符的运算符和分隔符
    switch (ch) {
        case ',': nextChar(); return Token(TokenType::COMMA, ",", tokenLine, tokenColumn);
        case ':': nextChar(); return Token(TokenType::COLON, ":", tokenLine, tokenColumn);
        case '[': nextChar(); return Token(TokenType::LEFT_BRACKET, "[", tokenLine, tokenColumn);
        case ']': nextChar(); return Token(TokenType::RIGHT_BRACKET, "]", tokenLine, tokenColumn);
        case '+': nextChar(); return Token(TokenType::PLUS, "+", tokenLine, tokenColumn);
        case '-': nextChar(); return Token(TokenType::MINUS, "-", tokenLine, tokenColumn);
        case '*': nextChar(); return Token(TokenType::MUL, "*", tokenLine, tokenColumn);
        case '/': nextChar(); return Token(TokenType::DIV, "/", tokenLine, tokenColumn);
        default: ;
    }
    if (ch == '%') {//预处理指令
        nextChar();
        std::string value;

        while (pos < length && !std::isspace(currentChar())) {
            value += currentChar();
            nextChar();
        }
        if (const auto it = std::ranges::lower_bound(macros,value);
            it != macros.end() && *it == value)
        return Token(TokenType::MACRO, value, tokenLine, tokenColumn);
    }
    //  识别操作码或寄存器（以字母开头）
    if (std::isalpha(ch)) {
        std::string value;
        while (pos < length && (
                std::isalpha(currentChar()) ||
                currentChar() == '_' ||
                std::isdigit(currentChar()
                ) )) {
            value += currentChar();
            nextChar();
        }

        // 判断是寄存器还是操作码
        // 寄存器格式：R后跟数字，且数字在0-254范围内
        if (value == "sp") {
            return Token(TokenType::SP_REG, value, tokenLine, tokenColumn);
        }
        if (value.length() > 1 && value[0] == 'r' && std::isdigit(value[1])) {
            std::string regNumStr = value.substr(1); // 提取R后面的数字部分
            // 检查是否为纯数字且在有效范围内
            bool isValidReg = true;
            for (char c : regNumStr) {
                if (!std::isdigit(c)) {
                    isValidReg = false;
                    break;
                }
            }
            if (isValidReg) {
                int regNum = std::stoi(regNumStr);
                if (regNum >= 0 && regNum <= 254) {
                    return Token(TokenType::REG, value, tokenLine, tokenColumn);
                }
            }
        }

        if (
            const auto it = std::ranges::lower_bound(ops,value);
            it != ops.end() && *it == value
            ) {
            return Token(TokenType::OPCODE, value, tokenLine, tokenColumn);
        }

        // 不满足条件，则认为是标志
        return Token(TokenType::IDENT, value, tokenLine, tokenColumn);
    }
    //  无法识别的字符
    std::string unknown(1, ch);
    nextChar();
    return Token(TokenType::UNKNOWN, unknown, tokenLine, tokenColumn);
}

// 分析整个源代码，返回Token列表
std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = getNextToken();
        tokens.push_back(token);

        if (token.type == TokenType::END) {
            break;
        }
    }

    return tokens;
}
}
