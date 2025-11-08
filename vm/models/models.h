#pragma once

// 基础对象
#include "object.h"

// 基本数据类型
#include "integer.h"
#include "boolean.h"
#include "./float.h"
#include "./string.h"
#include "null_object.h"

#include "array.h"
#include "value_object.h"

namespace lm {

    // 向后兼容
    using LmInteger = Integer;
    using LmString = String;
    using LmArray = Array;

} // namespace lm