//
// Created by geguj on 2025/10/12.
//

#pragma once
#include <memory>

enum class ObjectType {
    STRING,
    INTEGER,
    FLOAT,
    CALL_NATIVE
};
class LmObject {

public:
    LmObject(ObjectType type): m_type(type){}
    virtual ~LmObject() = default;

    ObjectType m_type;

    virtual void update_value(void* obj){}
};