#include <iostream>
#include <fstream>
#include <iterator>
#include <ranges>

#include "ir_generator.h"
#include "ir_token.h"

int main(int argc,char *argv[]) {
    if (argc < 2) {
        std::cout<<"Usage:\n";
        std::cout<<"./ir [ir_file]\n";
        return 0;
    }
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << argv[1] << std::endl;
        return 1;
    }
    auto str = std::string(std::istreambuf_iterator<char>(file),std::istreambuf_iterator<char>());
    file.close();
    ir::Lexer lexer(str);
    auto tokens = lexer.tokenize();
    ir::Generator gener(tokens);
    gener.build();
    for (const auto &f: gener.errs | std::views::keys) {
        std::cout << f << std::endl;
    }
    gener.saveToFile(argv[2]);

}
