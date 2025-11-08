#pragma once
#include "object.h"
#include <vector>
#include <memory>

namespace lm {

    class Array : public Object {
    public:
        Array();
        explicit Array(size_t size);
        ~Array() override = default;

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

        // 数组操作
        size_t size() const;
        bool empty() const;

        Object* get(size_t index) const;
        void set(size_t index, Object* value);
        void push(Object* value);
        void pop();
        void clear();

    private:
        std::vector<Object*> elements_;
        bool marked_ = false;
    };

} // namespace lm