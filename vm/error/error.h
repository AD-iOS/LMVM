#pragma once
#include <exception>
#include <stdexcept>


class LmError: public std::exception {
public:
    const char* m_message;
    LmError(const char* message): m_message(message) {}
    ~LmError() = default;
    const char* what() const noexcept override{return m_message;};
};