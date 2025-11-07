//
// Created by geguj on 2025/10/12.
//

#pragma once
#include <cstring>
#include "../error/error.h"
#include "Object.h"
#include "Types.h"

class LmString : public LmType {
    char* m_value;
    size_t size;
    size_t length;
public:
    
    explicit LmString(char* value);
    LmString(LmString&& other)noexcept;
    ~LmString() override = default;
    [[nodiscard]] char* to_ctype()const{return m_value;}
    [[nodiscard]] size_t len()const{return length;}

    bool operator!()const{return m_value == nullptr;}
    void operator=(char* V){
        if(m_value)
            delete[] m_value;
        length = std::strlen(V);
        auto remainder = length % 16;
        size = remainder == 0 ? length : (length + 16 - remainder);
        m_value = V;
    }

    void push(LmString &Str2);

    void reserve(size_t new_size);
};
