#pragma once
#include "object.h"

namespace lm {

    class NullObject : public Object {
    public:
        NullObject();
        ~NullObject() override = default;

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

    private:
        bool marked_ = false;
    };

} // namespace lm