#include "null_object.h"

namespace lm {

    NullObject::NullObject() = default;

    ObjectType NullObject::type() const {
        return ObjectType::NULL_OBJ;
    }

    std::string NullObject::type_name() const {
        return "Null";
    }

    TypeObject* NullObject::get_type() const {
        return nullptr; // 简化实现
    }

    std::string NullObject::to_string() const {
        return "null";
    }

    size_t NullObject::hash() const {
        return 0; // 所有null对象的哈希值相同
    }

    bool NullObject::equals(const Object* other) const {
        return dynamic_cast<const NullObject*>(other) != nullptr;
    }

    std::unique_ptr<Object> NullObject::clone() const {
        return std::make_unique<NullObject>();
    }

    void NullObject::mark() { marked_ = true; }
    bool NullObject::is_marked() const { return marked_; }

    std::vector<Object*> NullObject::get_references() const {
        return {};
    }

    std::string NullObject::debug_info() const {
        return "NullObject";
    }

} // namespace lm