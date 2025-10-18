//
// Created by geguj on 2025/10/17.
//

#pragma once
#pragma region LMVM_VERSION
#include <iostream>

#define PRINT(REG) std::cout << REG << '\n'
#define LMVM_VERSION "0.0.1"
#define LMVM_ALPHA_YEAR "2025"
#define LMVM_ALPHA_DATE "10-17"


inline auto format_out_version() {
    PRINT("Lamina-VM Version: " << LMVM_VERSION << "-dev");
    PRINT("Release Date: " << LMVM_ALPHA_YEAR << "-" << LMVM_ALPHA_DATE);
}
#undef PRINT
