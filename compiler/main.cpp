#include <iostream>
#include <fstream>
#include <iterator>
#include <ranges>

#include "ir/ir_generator.h"
#include "ir/ir_token.h"
#include "version.h"

int main(int argc,char *argv[]) {
    if (argc < 3) {
        std::cout << "\n    Usage: " << argv[0] << " [ir_file] [bc_file]\n\n";
        return 0;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << argv[1] << std::endl;
        return 1;
    }
    auto str = std::string(std::istreambuf_iterator(file),std::istreambuf_iterator<char>());
    file.close();
    ir::Lexer lexer(str);
    auto tokens = lexer.tokenize();
    ir::Generator gener(tokens);
    gener.build();
    for (const auto &f: gener.errs | std::views::keys) {
        std::cout << f << std::endl;
    }
    return gener.saveToFile(argv[2]);
}
