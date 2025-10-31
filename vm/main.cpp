#include "version.h"
#include "vm.h"
#include "error/error.h"
#include "op/Loader.h"

#include <chrono>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "\n    Usage: " << argv[0] << " [options] <file> [args]\n\n";
        format_out_version();
        return 0;
    }
    
    Loader loader(argv[1]);
    auto code = loader.load();
    const auto entry = loader.entry();

    VirtualMachine vm(code,argv);
    const auto start = std::chrono::high_resolution_clock::now();
    try {
        vm.run(entry);
    } catch (const LmError &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    const auto end = std::chrono::high_resolution_clock::now();
    const auto dur_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Finish in " << dur_ms << std::endl;
}