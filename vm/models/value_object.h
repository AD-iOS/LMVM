#pragma once
#include "object.h"
#include <memory>
#include <variant>

namespace lm {

    class ValueObject : public Object {
    public:
        // 构造函数
        ValueObject();
        explicit ValueObject(int64_t value);
        explicit ValueObject(bool value);
        explicit ValueObject(double value);
        explicit ValueObject(const std::string& value);
        explicit ValueObject(std::unique_ptr<Object> object);

        ~ValueObject() override = default;

        // Object接口实现
        ObjectType type() const override;
        std::string type_name() const override;
        TypeObject* get_type() const override;

        std::string to_string() const override;
        size_t hash() const override;
        bool equals(const Object* other) const override;
        std::unique_ptr<Object> clone() const override;

        void mark() override;
        bool is_marked() const override;
        std::vector<Object*> get_references() const override;
        std::string debug_info() const override;

        // 弱类型操作
        bool is_integer() const;
        bool is_boolean() const;
        bool is_float() const;
        bool is_string() const;
        bool is_null() const;

        // 类型转换
        int64_t as_integer() const;
        bool as_boolean() const;
        double as_float() const;
        std::string as_string() const;

        // 赋值操作
        void set_integer(int64_t value);
        void set_boolean(bool value);
        void set_float(double value);
        void set_string(const std::string& value);
        void set_null();
        void set_object(std::unique_ptr<Object> object);

        // 获取内部对象
        Object* get_object() const;

    private:
        std::unique_ptr<Object> value_;
        bool marked_ = false;
    };

} // namespace lm