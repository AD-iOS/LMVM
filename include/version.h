//
// Created by geguj on 2025/10/17.
//

#pragma once
#pragma region LMVM_VERSION
#pragma endregion
#include <iostream>

#define PRINT(REG) std::cout << REG << '\n'
#define LMVM_VERSION    "0.0.1"
#define LMVM_ALPHA_YEAR "2025"
#define LMVM_ALPHA_DATE "10-17"
#define LMVM_DEV        0x00000001
#define LMVM_MAGIC      0x4D4C5451;
constexpr static char magic_num_char[4] = {'Q','T','L','M'};
inline auto format_out_version() {
    PRINT("Lamina-VM Version: " << LMVM_VERSION << "-dev");
    PRINT("Release Date: " << LMVM_ALPHA_YEAR << "-" << LMVM_ALPHA_DATE);
}
#undef PRINT
