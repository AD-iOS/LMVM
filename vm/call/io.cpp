#include <cstdio>
#include <filesystem>

#include "../vm.h"
#include "../models/String.h"
#include "handler.h"

void Handler::Lm_io_write(VirtualMachine* vm){
    const auto address = dynamic_pointer_cast<LmString>(vm->heapManager->loadObject(vm->reg[3]));
    if (address == nullptr)throw LmError("vmcall[write] Error: pointer is not LmString");
    fputs(address->to_ctype(),stdout);
}
void Handler::Lm_io_read(VirtualMachine* vm){
    const auto address = dynamic_pointer_cast<LmString>(vm->heapManager->loadObject(vm->reg[3]));
    if (address == nullptr)throw LmError("vmcall[write] Error: pointer is not LmString");
    fgets(address->to_ctype(),1024,stdin);
}