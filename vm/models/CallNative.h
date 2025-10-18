#pragma once
#include "Object.h"
#include <functional>

class VirtualMachine;

using NativeFuncPtr = std::function<void(VirtualMachine*)>;
class LmCallNative: public LmObject {
    void* hDLL; 
    std::unordered_map<std::string, NativeFuncPtr> func_map;
    
public:
    explicit LmCallNative(const char* path);
    ~LmCallNative() override = default;
    void unCallNative();
    [[nodiscard]] bool is_loaded() const;

    NativeFuncPtr get_func(const char* name) const;

    static void load_dll();
    static void unload_dll();
    static void get_func_ptr();
    static void Call();
};