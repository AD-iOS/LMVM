#pragma once
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace ir {
    enum TokenType{
        END,
        IDENT,
        INSTR,
        NUM,
        COMMA,

        PLUS,
        MINUS,
        MUL,
        DIV,
        
        REG,

        COLON
    };
    struct Token {
        TokenType type;
        std::string value;
        size_t line,column;

        Token(TokenType type,std::string& value,size_t line,size_t column):
            type(type),
            value(value),
            line(line),
            column(column){}
    };
    class Lexer {
    public:
      Lexer(std::stringstream &source)
          : source(source), line(1), column(0), pos(0) {}
      std::vector<Token> tokenize();
      static std::vector<std::string> instrs;

    private:
      char getChar();
      void ungetChar();
      void handleStringLiteral(std::string &str, int startLine,
                               int startColumn);
      void skipWhitespaceAndComments();
      Token getNextToken();

      bool hasUnget;
      std::stringstream &source;
      size_t line, column;
      char lastChar;
      char pos;
      std::string lastKeyWord;
    };
};