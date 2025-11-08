#include "integer.h"
#include "type_object.h"

namespace lm {

    Integer::Integer(int64_t value) : value_(value) {}

    ObjectType Integer::type() const {
        return ObjectType::INTEGER;
    }

    std::string Integer::type_name() const {
        return "Integer";
    }

    TypeObject* Integer::get_type() const {
        // 返回对应的类型对象
        return nullptr; // 简化实现
    }

    std::string Integer::to_string() const {
        return std::to_string(value_);
    }

    size_t Integer::hash() const {
        return std::hash<int64_t>{}(value_);
    }

    bool Integer::equals(const Object* other) const {
        if (auto int_obj = dynamic_cast<const Integer*>(other)) {
            return value_ == int_obj->value_;
        }
        return false;
    }

    std::unique_ptr<Object> Integer::clone() const {
        return std::make_unique<Integer>(value_);
    }

    void Integer::mark() { marked_ = true; }
    bool Integer::is_marked() const { return marked_; }

    std::vector<Object*> Integer::get_references() const {
        return {}; // 基本类型没有引用其他对象
    }

    std::string Integer::debug_info() const {
        return "Integer[" + std::to_string(value_) + "]";
    }

    int64_t Integer::value() const { return value_; }
    void Integer::set_value(int64_t value) { value_ = value; }

} // namespace lm