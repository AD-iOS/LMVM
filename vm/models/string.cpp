#include "string.h"

namespace lm {

    String::String() : value_("") {}

    String::String(const char* str) : value_(str ? str : "") {}

    String::String(const std::string& str) : value_(str) {}

    ObjectType String::type() const {
        return ObjectType::STRING;
    }

    std::string String::type_name() const {
        return "String";
    }

    TypeObject* String::get_type() const {
        return nullptr; // 简化实现
    }

    std::string String::to_string() const {
        return value_;
    }

    size_t String::hash() const {
        return std::hash<std::string>{}(value_);
    }

    bool String::equals(const Object* other) const {
        if (auto str_obj = dynamic_cast<const String*>(other)) {
            return value_ == str_obj->value_;
        }
        return false;
    }

    std::unique_ptr<Object> String::clone() const {
        return std::make_unique<String>(value_);
    }

    void String::mark() { marked_ = true; }
    bool String::is_marked() const { return marked_; }

    std::vector<Object*> String::get_references() const {
        return {};
    }

    std::string String::debug_info() const {
        return "String[\"" + value_ + "\"]";
    }

    const char* String::c_str() const {
        return value_.c_str();
    }

    size_t String::length() const {
        return value_.length();
    }

    bool String::empty() const {
        return value_.empty();
    }

    const std::string& String::value() const {
        return value_;
    }

    char String::at(size_t index) const {
        return index < value_.length() ? value_[index] : '\0';
    }

    void String::append(const char* str) {
        if (str) {
            value_ += str;
        }
    }

    void String::append(const std::string& str) {
        value_ += str;
    }

    char *String::to_ctype() const {
        return const_cast<char*>(value_.c_str());
    }


} // namespace lm