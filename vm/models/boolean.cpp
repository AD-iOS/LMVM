#include "boolean.h"

namespace lm {

    Boolean::Boolean(bool value) : value_(value) {}

    ObjectType Boolean::type() const {
        return ObjectType::BOOLEAN;
    }

    std::string Boolean::type_name() const {
        return "Boolean";
    }

    TypeObject* Boolean::get_type() const {
        return nullptr; // 简化实现
    }

    std::string Boolean::to_string() const {
        return value_ ? "true" : "false";
    }

    size_t Boolean::hash() const {
        return std::hash<bool>{}(value_);
    }

    bool Boolean::equals(const Object* other) const {
        if (auto bool_obj = dynamic_cast<const Boolean*>(other)) {
            return value_ == bool_obj->value_;
        }
        return false;
    }

    std::unique_ptr<Object> Boolean::clone() const {
        return std::make_unique<Boolean>(value_);
    }

    void Boolean::mark() { marked_ = true; }
    bool Boolean::is_marked() const { return marked_; }

    std::vector<Object*> Boolean::get_references() const {
        return {};
    }

    std::string Boolean::debug_info() const {
        return "Boolean[" + std::string(value_ ? "true" : "false") + "]";
    }

    bool Boolean::value() const { return value_; }
    void Boolean::set_value(bool value) { value_ = value; }

} // namespace lm