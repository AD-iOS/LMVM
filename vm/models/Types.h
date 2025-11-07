//
// Created by geguj on 2025/11/3.
//

#pragma once
#include "Object.h"


class LmType : LmObject {
public:
    enum class TypeKind {
        Int, Float, Bool, Array, Struct, Null
    };
    explicit LmType(const TypeKind kind) :
        LmObject(ObjectType::TYPES),
        kind(kind) {}

    ~LmType() override = default;
    [[nodiscard]] virtual std::string _name_() const = 0;
    virtual bool is_equal(const TypeKind k) {return kind == k;}

    std::string get_object_type_str()override {
        switch (kind) {
                case TypeKind::Int:return "Int";
                case TypeKind::Float:return "Float";
                case TypeKind::Bool:return "Bool";
                case TypeKind::Array:return "Array";
                case TypeKind::Struct:return "Struct";
                case TypeKind::Null:return "Null";
        }
        return "Void Type";
    }

private:
    TypeKind kind;
};