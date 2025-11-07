//
// Created by geguj on 2025/11/2.
//
#include "console.h"

const char* Console::lm_options[] = {
    "help","compile", "run", "debug"
};
Console::Console(const int argc, char **argv) {
    for (size_t i = 0; i < sizeof(lm_options); i++) {
        if (lm_options[i] == argv[1]) option = lm_options[i];
    }
    for (size_t i = 0; i < argc; i++) {
        if (argv[0][0] == '-' && std::string(argv[i]).find('-') <= 1) {
            //如果前两个字符包含-
            compile_args.emplace_back(argv[i]);
        } else file_paths.emplace_back(argv[i]);
    }
    if (option.empty()) option = lm_options[2]; //无则为运行
}

void Console::compile_ir2bc() {

}
