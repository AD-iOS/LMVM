//
// Created by geguj on 2025/10/18.
//

#pragma once
#include <cstdint>


#include "ir_token.h"

namespace ir {
    class Generator {
        size_t pos;
        std::vector<Token>& tokens;
        std::vector<uint8_t> code;
    public:
        Generator(std::vector<Token>& tokens);
        ~Generator();
        void build();
    private:
        void next_token();
        Token& cur_token();
        void gen_mov();
    };
}