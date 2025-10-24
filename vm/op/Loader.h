//
// Created by geguj on 2025/10/12.
//

#pragma once
#include "OpCode.h"
#include "version.h"
#include <cstdint>
#include <fstream>


class Loader {
    struct Header{
        uint32_t magic_num;
        uint32_t version;
        uint64_t entry;
    };
    Header h;
    std::ifstream m_file;
public:
    Loader(const std::string& path);
    ~Loader();
    Op load_op();
    std::vector<Op> load();

    uint64_t entry() const { return h.entry; }

    bool check_header();

    
private:
    static const uint32_t MAGIC = 0x4D4C5451;   // Little Endian 'QTLM'
    static const uint32_t VERSION = LMVM_DEV; // current version
};