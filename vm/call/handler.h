#pragma once
#include <functional>

#include "../vm.h"
namespace Handler
{
    void Lm_io_write(VirtualMachine *vm);
    void Lm_io_read(VirtualMachine *vm);
    void Lm_exit(VirtualMachine *vm);
    void Lm_get_args(VirtualMachine *vm);

    using VmCall =  void (*)(VirtualMachine *);
    static VmCall vmcallTable[] = {
            [](VirtualMachine *vm) {
                Lm_io_write(vm);
            },
            [](VirtualMachine *vm) {
                Lm_io_read(vm);
            },
            [](VirtualMachine *vm) {
                Lm_exit(vm);
            },
    };

    static int maxVMCallNum = sizeof(vmcallTable) / sizeof(vmcallTable[0]);
};
