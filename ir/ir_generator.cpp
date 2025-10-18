//
// Created by geguj on 2025/10/18.
//

#include "ir_generator.h"
#include <span>
#include "../vm/op/OpCode.h"

namespace ir {
    Generator::Generator(std::vector<Token>& tokens) : pos(0), tokens(tokens) {
    }

    Generator::~Generator() = default;

    void Generator::next_token() {
        if (pos <= tokens.size())
        pos++;
        else pos = -1;
    }
    Token& Generator::cur_token() {
        return pos != -1 ? tokens[pos] : tokens.back();
    }
    void Generator::gen_mov() {
        //todo
        switch (cur_token().type) {
            case TokenType::LEFT_BRACKET: {// m

            }
            case TokenType::REG: {// r

            }
            case TokenType::SP_REG: {//sp

            }
            default: {

            }
        }
    }


}