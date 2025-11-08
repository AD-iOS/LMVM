#pragma once
#include "object.h"

namespace lm {

    class Integer : public Object {
    public:
        explicit Integer(int64_t value = 0);
        ~Integer() override = default;

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

        // 特定接口
        int64_t value() const;
        void set_value(int64_t value);
        int64_t to_ctype()const {return value_;}
        void update_value(const int64_t value){value_ = value;}

    private:
        int64_t value_;
        bool marked_ = false;
    };

} // namespace lm