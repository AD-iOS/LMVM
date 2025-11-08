#include "array.h"

namespace lm {

Array::Array() = default;

Array::Array(size_t size) : elements_(size, nullptr) {}

ObjectType Array::type() const { 
    return ObjectType::ARRAY; 
}

std::string Array::type_name() const { 
    return "Array"; 
}

TypeObject* Array::get_type() const {
    return nullptr; // 简化实现
}

std::string Array::to_string() const {
    std::string result = "[";
    for (size_t i = 0; i < elements_.size(); ++i) {
        if (i > 0) result += ", ";
        result += elements_[i] ? elements_[i]->to_string() : "null";
    }
    result += "]";
    return result;
}

size_t Array::hash() const {
    size_t h = 0;
    for (Object* elem : elements_) {
        h ^= elem ? elem->hash() : 0;
    }
    return h;
}

bool Array::equals(const Object* other) const {
    if (auto arr_obj = dynamic_cast<const Array*>(other)) {
        if (elements_.size() != arr_obj->elements_.size()) return false;
        
        for (size_t i = 0; i < elements_.size(); ++i) {
            Object* elem1 = elements_[i];
            Object* elem2 = arr_obj->elements_[i];
            
            if (!elem1 != !elem2) return false; // 一个为null，另一个不为null
            if (elem1 && !elem1->equals(elem2)) return false;
        }
        return true;
    }
    return false;
}

std::unique_ptr<Object> Array::clone() const {
    auto new_array = std::make_unique<Array>();
    for (Object* elem : elements_) {
        new_array->elements_.push_back(elem ? elem->clone().release() : nullptr);
    }
    return new_array;
}

void Array::mark() {
    marked_ = true;
    for (Object* elem : elements_) {
        if (elem) elem->mark();
    }
}

bool Array::is_marked() const {
    return marked_;
}

std::vector<Object*> Array::get_references() const {
    std::vector<Object*> refs;
    for (Object* elem : elements_) {
        if (elem) refs.push_back(elem);
    }
    return refs;
}

std::string Array::debug_info() const {
    return "Array[size=" + std::to_string(elements_.size()) + "]";
}

size_t Array::size() const {
    return elements_.size();
}

bool Array::empty() const {
    return elements_.empty();
}

Object* Array::get(size_t index) const {
    return index < elements_.size() ? elements_[index] : nullptr;
}

void Array::set(size_t index, Object* value) {
    if (index < elements_.size()) {
        elements_[index] = value;
    }
}

void Array::push(Object* value) {
    elements_.push_back(value);
}

void Array::pop() {
    if (!elements_.empty()) {
        elements_.pop_back();
    }
}

void Array::clear() {
    elements_.clear();
}

} // namespace lm