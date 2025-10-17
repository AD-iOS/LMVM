//
// Created by geguj on 2025/10/12.
//

#pragma once

enum class ObjectType {
    STRING,
    INTEGER,
    FLOAT
};
class LmObject {

public:
    LmObject(ObjectType type): m_type(type){}
    virtual ~LmObject() = default;

    ObjectType m_type;

    virtual void update_value(void* value){}
};