#pragma once
#include <exception>
#include <stdexcept>
#include <string>
#include <cstdarg>
#include <vector>

class LmError: public std::exception {
public:
    explicit LmError(const char* message): m_message(message) {}
    explicit LmError(const std::string& message): m_message(message) {}
    ~LmError() override = default;

    const char* what() const noexcept override { return m_message.c_str(); }

    // 使用 printf 风格格式化创建 LmError（安全地分配缓冲并拷贝）
    static LmError format(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);

        // 先计算需要的长度
        va_list args_copy;
        va_copy(args_copy, args);
        int required = std::vsnprintf(nullptr, 0, fmt, args_copy);
        va_end(args_copy);

        if (required < 0) {
            va_end(args);
            return LmError("Formatting error");
        }

        std::vector<char> buf(static_cast<size_t>(required) + 1);
        std::vsnprintf(buf.data(), buf.size(), fmt, args);
        va_end(args);

        return LmError(std::string(buf.data(), static_cast<size_t>(required)));
    }

private:
    std::string m_message;
};