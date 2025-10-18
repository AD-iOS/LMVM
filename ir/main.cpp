#include <iostream>
#include <fstream>
#include <iterator>
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
    std::cout << str << '\n';

    ir::Lexer lexer(str);
    auto tokens = lexer.tokenize();
    for (auto token : tokens) {
        std::cout << token.value << "\t:\t" << token.toString() << '\n';
    }
    file.close();
}
