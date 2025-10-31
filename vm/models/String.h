//
// Created by geguj on 2025/10/12.
//

#pragma once
#include <cstring>
#include "../error/error.h"
#include "Object.h"

class LmString : public LmObject {
    char *m_value;
    size_t size;
    size_t length;
public:
    
    LmString(char* value);
    LmString(LmString&& other)noexcept;
    ~LmString() = default;
    char* to_ctype()const{return m_value;}
    size_t len()const{return length;}

    bool operator!()const{return m_value == nullptr;}
    void operator=(char* _V){
        if(m_value)
            delete[] m_value;
        length = std::strlen(_V);
        auto remainder = length % 16;
        size = remainder == 0 ? length : (length + 16 - remainder);
        m_value = _V;
    }

    void push(LmString &_Str2);

    void reserve(size_t new_size);
};