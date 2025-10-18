#pragma once
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ir {
    enum class TokenType {
        END, REG, SP_REG, IDENT, NUM, UNKNOWN, OPCODE, COMMENT,

        COMMA, COLON, LEFT_BRACKET, RIGHT_BRACKET,
        PLUS, MINUS, MUL, DIV, MACRO
    };

    struct Token {
        TokenType type;
        std::string value;
        size_t line, column;

        Token(const TokenType type, std::string value, const size_t line, const size_t column) :
            type(type),
            value(std::move(value)),
            line(line),
            column(column) {
        }

        Token():type(TokenType::END),value({}),line(0),column(0){}

        std::string toString() const;
    };

    class Lexer {
    public:
        static std::vector<std::string> ops;
        static std::vector<std::string> macros;
        Lexer(std::string& source);
        std::vector<Token> tokenize();
        std::string parse_comment();



    private:
        char currentChar() const;
        void nextChar();
        void skipWhitespace();
        Token getNextToken();

        std::string &source;
        size_t pos;
        size_t length;
        size_t currentLine;
        size_t currentColumn;



    };
};
