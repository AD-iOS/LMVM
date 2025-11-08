#pragma once
#include "object.h"

namespace lm {

    class Boolean : public Object {
    public:
        explicit Boolean(bool value = false);
        ~Boolean() override = default;

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

        bool value() const;
        void set_value(bool value);

    private:
        bool value_;
        bool marked_ = false;
    };

} // namespace lm