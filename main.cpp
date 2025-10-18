#include "vm/error/error.h"
#include "vm/op/Loader.h"
#include "vm/vm.h"
#include "include/version.h"
#include <iostream>


int main(int argc, char* argv[]) {
    if (argc < 2){
        std::cout << "Usage: " << argv[0] << " [options] <file> [args]\n";
        format_out_version();
        return 0;
    }

    Loader loader(argv[1]);
    auto code = loader.load();
    const auto entry = loader.entry();
    // std::vector<Op> code = {
    //     {OpCode::NEWSTR,{3,'H','e','l','l','o','\n','\0'}},
    //     {OpCode::VMCALL,{0,0}}
    // };
    VirtualMachine vm(code);
    try{
        vm.run(entry);
    }catch (const LmError &e){
        std::cout << "Error: " << e.what() << std::endl;
    }
}