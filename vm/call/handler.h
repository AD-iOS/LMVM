#pragma once
#include <functional>

#include "../vm.h"
namespace Handler
{
    inline void Lm_io_write(VirtualMachine* vm);
    inline void Lm_io_read(VirtualMachine* vm);


    static inline std::function<void(VirtualMachine*)> vmcallTable[] = {
        [](VirtualMachine* vm) {
            Lm_io_write(vm);
        },
        [](VirtualMachine* vm) {
            Lm_io_read(vm);
        }
    };

};
