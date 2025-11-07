//
// Created by geguj on 2025/10/12.
//

#pragma once
#include <memory>

enum class ObjectType {
    TYPES,
    CALL_NATIVE
};
class LmObject {
    ObjectType m_type;
public:
    explicit LmObject(ObjectType type): m_type(type){}
    virtual ~LmObject() = default;



    virtual void update_value(void* obj){}
    virtual ObjectType get_object_type(){return m_type;}
    virtual std::string get_object_type_str(){return "LmObject";}
};