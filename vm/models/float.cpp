#include "float.h"
#include <sstream>
#include <iomanip>

namespace lm {

    Float::Float(double value) : value_(value) {}

    ObjectType Float::type() const {
        return ObjectType::FLOAT;
    }

    std::string Float::type_name() const {
        return "Float";
    }

    TypeObject* Float::get_type() const {
        return nullptr; // 简化实现
    }

    std::string Float::to_string() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << value_;
        std::string result = oss.str();

        // 移除末尾多余的0和小数点
        auto pos = result.find_last_not_of('0');
        if (pos != std::string::npos && result[pos] == '.') {
            pos--;
        }
        if (pos != std::string::npos) {
            result = result.substr(0, pos + 1);
        }

        return result;
    }

    size_t Float::hash() const {
        return std::hash<double>{}(value_);
    }

    bool Float::equals(const Object* other) const {
        if (auto float_obj = dynamic_cast<const Float*>(other)) {
            // 浮点数比较需要处理精度问题
            return std::abs(value_ - float_obj->value_) < 1e-10;
        }
        return false;
    }

    std::unique_ptr<Object> Float::clone() const {
        return std::make_unique<Float>(value_);
    }

    void Float::mark() { marked_ = true; }
    bool Float::is_marked() const { return marked_; }

    std::vector<Object*> Float::get_references() const {
        return {};
    }

    std::string Float::debug_info() const {
        return "Float[" + to_string() + "]";
    }

    double Float::value() const { return value_; }
    void Float::set_value(double value) { value_ = value; }

} // namespace lm