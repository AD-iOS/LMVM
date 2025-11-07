//
// Created by geguj on 2025/11/2.
//

#pragma once
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


class Console {
    std::string option;

    std::vector<char*>
        file_paths, compile_args;
public:
    static const char* lm_options[];
    Console(int argc, char** argv);
    ~Console() = default;

    void compile_ir2bc(std::string& path);

};
