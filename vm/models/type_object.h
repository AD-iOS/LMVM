#pragma once
#include "object.h"
#include <unordered_map>
#include <typeinfo>

namespace lm {

/**
 * @brief 类型对象，表示类型的元信息
 */
class TypeObject : public Object {
private:
    std::string name_;
    size_t size_;
    std::unordered_map<std::string, Object*> attributes_;
    TypeObject* base_type_;

public:
    TypeObject(const std::string& name, size_t size, TypeObject* base = nullptr)
        : Object(ObjectType::TYPE_OBJECT), name_(name), size_(size), base_type_(base) {}

    std::string to_string() const override {
        return "<type '" + name_ + "'>";
    }

    size_t hash() const override {
        return std::hash<std::string>{}(name_);
    }

    bool equals(const Object* other) const override {
        if (this == other) return true;
        if (auto type_obj = dynamic_cast<const TypeObject*>(other)) {
            return name_ == type_obj->name_;
        }
        return false;
    }


    // 类型系统操作
    const std::string& name() const { return name_; }
    size_t instance_size() const { return size_; }
    TypeObject* base_type() const { return base_type_; }

    bool is_subtype_of(const TypeObject* other) const {
        const TypeObject* current = this;
        while (current) {
            if (current == other) return true;
            current = current->base_type_;
        }
        return false;
    }

    // 属性管理
    void set_attribute(const std::string& name, Object* value) {
        attributes_[name] = value;
    }

    Object* get_attribute(const std::string& name) const {
        auto it = attributes_.find(name);
        return it != attributes_.end() ? it->second : nullptr;
    }
};
}