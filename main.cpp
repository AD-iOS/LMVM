#include "vm/error/error.h"
#include "vm/op/Loader.h"
#include "vm/vm.h"
#include "include/version.h"

#include <iostream>
#include <chrono>


int main(char *argv[]) {
    if (argc < 2){
        std::cout << "Usage: " << argv[0] << " [options] <file> [args]\n";
        format_out_version();
        return 0;
    }

    Loader loader(argv[1]);
    auto code = loader.load();
    const auto entry = loader.entry();

    VirtualMachine vm(code);
    try{
        auto start = std::chrono::high_resolution_clock::now();
        vm.run(0);
        for (size_t i=0;i!=100000000;i++)
            vm.run(1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double,std::ratio<1,1000>> dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << dur_ms << "\n";
    }catch (const LmError &e){
        std::cout << "Error: " << e.what() << std::endl;
    }
}