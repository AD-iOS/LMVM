//
// Created by geguj on 2025/10/12.
//

#include "Integer.h"

// 添加移动构造函数以提高性能
LmInteger::LmInteger(LmInteger&& other) noexcept 
    : LmObject(ObjectType::INTEGER), 
      m_signed(other.m_signed) {
    std::memcpy(m_value, other.m_value, 8);
    // 将源对象置为无效状态
    other.m_signed = false;
}