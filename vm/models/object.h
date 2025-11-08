#pragma once
#include <string>
#include <vector>
#include <memory>

namespace lm {

    // 对象类型枚举
    enum class ObjectType {
        INTEGER, BOOLEAN, FLOAT, STRING,
        ARRAY, DICT, FUNCTION, NULL_OBJ, VALUE, TYPE_OBJECT
    };

    // 前向声明
    class TypeObject;
    class GarbageCollector;

    /**
     * @brief 所有对象的基类
     */
    class Object {
    public:
        virtual ~Object() = default;

        // 类型信息
        virtual ObjectType type() const = 0;
        virtual std::string type_name() const = 0;
        virtual TypeObject* get_type() const = 0;

        // 通用接口
        virtual std::string to_string() const = 0;
        virtual size_t hash() const = 0;
        virtual bool equals(const Object* other) const = 0;
        virtual std::unique_ptr<Object> clone() const = 0;

        // 垃圾回收支持
        virtual void mark() = 0;
        virtual bool is_marked() const = 0;
        virtual std::vector<Object*> get_references() const = 0;

        // 调试信息
        virtual std::string debug_info() const = 0;

        Object() = default;

        explicit Object(ObjectType object);
    };

} // namespace lm