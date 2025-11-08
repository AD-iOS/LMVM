#include "value_object.h"
#include "integer.h"
#include "boolean.h"
#include "float.h"
#include "string.h"
#include "null_object.h"

namespace lm {

ValueObject::ValueObject() : value_(std::make_unique<NullObject>()) {}

ValueObject::ValueObject(int64_t value) 
    : value_(std::make_unique<Integer>(value)) {}

ValueObject::ValueObject(bool value) 
    : value_(std::make_unique<Boolean>(value)) {}

ValueObject::ValueObject(double value) 
    : value_(std::make_unique<Float>(value)) {}

ValueObject::ValueObject(const std::string& value) 
    : value_(std::make_unique<String>(value)) {}

ValueObject::ValueObject(std::unique_ptr<Object> object) 
    : value_(std::move(object)) {}

ObjectType ValueObject::type() const { 
    return ObjectType::VALUE; 
}

std::string ValueObject::type_name() const { 
    return "ValueObject"; 
}

TypeObject* ValueObject::get_type() const {
    return value_ ? value_->get_type() : nullptr;
}

std::string ValueObject::to_string() const {
    return value_ ? value_->to_string() : "null";
}

size_t ValueObject::hash() const {
    return value_ ? value_->hash() : 0;
}

bool ValueObject::equals(const Object* other) const {
    if (auto val_obj = dynamic_cast<const ValueObject*>(other)) {
        if (!value_ && !val_obj->value_) return true;
        if (value_ && val_obj->value_) return value_->equals(val_obj->value_.get());
        return false;
    }
    return false;
}

std::unique_ptr<Object> ValueObject::clone() const {
    return value_ ? std::make_unique<ValueObject>(value_->clone()) 
                  : std::make_unique<ValueObject>();
}

void ValueObject::mark() { 
    marked_ = true;
    if (value_) value_->mark();
}

bool ValueObject::is_marked() const { 
    return marked_; 
}

std::vector<Object*> ValueObject::get_references() const {
    return value_ ? std::vector<Object*>{value_.get()} : std::vector<Object*>{};
}

std::string ValueObject::debug_info() const {
    return "ValueObject[" + (value_ ? value_->debug_info() : "null") + "]";
}

bool ValueObject::is_integer() const {
    return value_ && value_->type() == ObjectType::INTEGER;
}

bool ValueObject::is_boolean() const {
    return value_ && value_->type() == ObjectType::BOOLEAN;
}

bool ValueObject::is_float() const {
    return value_ && value_->type() == ObjectType::FLOAT;
}

bool ValueObject::is_string() const {
    return value_ && value_->type() == ObjectType::STRING;
}

bool ValueObject::is_null() const {
    return !value_ || value_->type() == ObjectType::NULL_OBJ;
}

int64_t ValueObject::as_integer() const {
    if (auto int_obj = dynamic_cast<Integer*>(value_.get())) {
        return int_obj->value();
    }
    return 0;
}

bool ValueObject::as_boolean() const {
    if (auto bool_obj = dynamic_cast<Boolean*>(value_.get())) {
        return bool_obj->value();
    }
    return false;
}

double ValueObject::as_float() const {
    if (auto float_obj = dynamic_cast<Float*>(value_.get())) {
        return float_obj->value();
    }
    return 0.0;
}

std::string ValueObject::as_string() const {
    if (auto str_obj = dynamic_cast<String*>(value_.get())) {
        return str_obj->value();
    }
    return "";
}

void ValueObject::set_integer(int64_t value) {
    value_ = std::make_unique<Integer>(value);
}

void ValueObject::set_boolean(bool value) {
    value_ = std::make_unique<Boolean>(value);
}

void ValueObject::set_float(double value) {
    value_ = std::make_unique<Float>(value);
}

void ValueObject::set_string(const std::string& value) {
    value_ = std::make_unique<String>(value);
}

void ValueObject::set_null() {
    value_ = std::make_unique<NullObject>();
}

void ValueObject::set_object(std::unique_ptr<Object> object) {
    value_ = std::move(object);
}

Object* ValueObject::get_object() const {
    return value_.get();
}

} // namespace lm