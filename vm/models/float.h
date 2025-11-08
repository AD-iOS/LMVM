#pragma once
#include "object.h"

namespace lm {

    class Float : public Object {
    public:
        explicit Float(double value = 0.0);
        ~Float() override = default;

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

        double value() const;
        void set_value(double value);

    private:
        double value_;
        bool marked_ = false;
    };

} // namespace lm