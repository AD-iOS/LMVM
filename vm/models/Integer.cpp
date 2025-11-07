//
// Created by geguj on 2025/10/12.
//

#include "Integer.h"

// 添加移动构造函数以提高性能
LmInteger::LmInteger(LmInteger&& other) noexcept 
    : LmType(TypeKind::Int),
      m_signed(other.m_signed) {
    std::memcpy(m_value, other.m_value, 8);
}