//
// Created by geguj on 2025/10/12.
// 仅作为push pop操作的heap载体
//

#pragma once
#include <bit>
#include <cstdint>
#include <cstring>
#include "Object.h"

class LmInteger: public LmObject {
    bool m_signed;
    
public:
    uint8_t m_value[8]{};
    LmInteger(uint8_t value[8], const bool is_signed):LmObject(ObjectType::INTEGER),m_signed(is_signed){
        std::memcpy(m_value, value, 8);
    }

    explicit LmInteger(const uint64_t value):LmObject(ObjectType::INTEGER),m_signed(false){
        std::memcpy(m_value, &value, 8);
    }

    explicit LmInteger(int64_t value):LmObject(ObjectType::INTEGER), m_signed(true){
        value = std::bit_cast<uint64_t>(value);
        std::memcpy(m_value, &value, 8);
    }

    LmInteger(LmInteger &&other) noexcept;


    uint64_t to_ctype(){
        return *reinterpret_cast<uint64_t *>(m_value);
    }
    uint64_t* get_ptr(){
        return reinterpret_cast<uint64_t *>(m_value);
    }
    void update_value(void* value_ptr)override {
        std::memcpy(m_value, value_ptr, 8);
    }
    void update_value(const uint64_t v) {
        std::memcpy(m_value, &v, sizeof(v));
    }
    void update_value(const int64_t v) {
        std::memcpy(m_value, &v, sizeof(v));
    }
};