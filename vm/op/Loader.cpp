//
// Created by geguj on 2025/10/12.
//

#include "Loader.h"

Loader::Loader(const std::string& path) {
    m_file.open(path, std::ios::binary);
    if (!m_file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

}
Loader::~Loader(){
    if(m_file.is_open())m_file.close();
}

bool Loader::check_header() {
    m_file.seekg(0, std::ios::beg);
    m_file.read(reinterpret_cast<char*>(&h), sizeof(h));
    if (m_file.gcount() != sizeof(h)){
        m_file.close();
        throw std::runtime_error("Invalid header");
    }
    return h.magic_num == MAGIC && h.version == VERSION;
}

Op Loader::load_op() {
    Op op;
    op.op = static_cast<OpCode>(m_file.get());
    const auto data_size = m_file.get();
    op.data.resize(data_size);
    m_file.read(reinterpret_cast<char*>(op.data.data()), data_size);

    return std::move(op);
}

std::vector<Op> Loader::load() {
    if (!check_header()) {
        throw std::runtime_error("Invalid header");
    }
    std::vector<Op> ops;
    for (size_t i = 0; m_file.peek() != EOF; i++) {
        ops.push_back(load_op());
    }
    return std::move(ops);
}