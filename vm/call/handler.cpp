#include "handler.h"
#include <cstdio>
#include <iostream>

void Handler::Lm_exit(VirtualMachine *vm)
{
    int code = static_cast<int>(vm->reg[3]);
    // 抛出异常以触发主程序的统一错误处理路径
    throw LmError::format("Terminated before completion (code %d)", code);
}
