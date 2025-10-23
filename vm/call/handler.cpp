#include "handler.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include "../vm.h"
#include "../models/String.h"

void Handler::Lm_io_write(VirtualMachine* vm){

    if (const auto address = std::dynamic_pointer_cast<LmString>(
        vm->heapManager->
        loadObject(vm->reg[3])
    ))fputs(address->to_ctype(),stdout);

    throw LmError("vmcall[write] Error: pointer is not LmString");
}
void Handler::Lm_io_read(VirtualMachine* vm){

    if (const auto address = std::dynamic_pointer_cast<LmString>(
        vm->heapManager->
        loadObject(vm->reg[3])
    ))fgets(address->to_ctype(),1024,stdin);
    
    throw LmError("vmcall[read] Error: pointer is not LmString");
    
}
void Handler::Lm_exit(VirtualMachine *vm)
{
    int code = static_cast<int>(vm->reg[3]);
    // 抛出异常以触发主程序的统一错误处理路径
    throw LmError::format("Terminated before completion (code %d)",code);
}
