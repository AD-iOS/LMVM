#pragma once
#include "object.h"
#include <string>

namespace lm {

    class String : public Object {
    public:
        String();
        explicit String(const char* str);
        explicit String(const std::string& str);
        ~String() override = default;

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

        // 字符串操作
        const char* c_str() const;
        size_t length() const;
        bool empty() const;
        const std::string& value() const;

        char at(size_t index) const;
        void append(const char* str);
        void append(const std::string& str);

        char *to_ctype() const;

    private:
        std::string value_;
        bool marked_ = false;
    };

} // namespace lm