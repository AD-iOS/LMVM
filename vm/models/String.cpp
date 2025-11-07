//
// Created by geguj on 2025/10/12.
//

#include "String.h"

void LmString::reserve(size_t new_size) {
  if (new_size <= size || new_size % 16 != 0)
    return;
  char *new_value = new char[new_size];
  memcpy(new_value, m_value, size);
  delete[] m_value;
  m_value = new_value;
  size = new_size;
}

void LmString::push(LmString& _Str2) {
    if (!_Str2)
        return;
    if (size - length < _Str2.len()) 
        reserve((size + _Str2.len()) * 2);

    memcpy(m_value + length, _Str2.to_ctype(), _Str2.len());
    length += _Str2.len();
    _Str2.m_value = nullptr;
}

// 添加移动构造函数以提高性能
LmString::LmString(LmString&& other) noexcept 
    : LmType(TypeKind::Array),
      m_value(other.m_value),
      size(other.size),
      length(other.length) {
    // 将源对象置为无效状态
    other.m_value = nullptr;
    other.size = 0;
    other.length = 0;
}
LmString::LmString(char* value):
        LmType(TypeKind::Array),
        m_value(value),
        length(std::strlen(value)){
            auto remainder = length % 16;
            size = remainder == 0 ? length : (length + 16 - remainder);
        }