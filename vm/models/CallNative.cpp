#include "CallNative.h"
#include <iostream>
#include "Object.h"
#if _WIN32 || _WIN64
#include <windows.h>
#else
#include <dlfcn.h>
#endif

LmCallNative::LmCallNative(const char* path): hDLL(nullptr),LmObject(ObjectType::CALL_NATIVE){
    #if _WIN64
    hDLL = LoadLibrary(path);
    if (!hDLL) {
        throw LmError("Failed to load library");
    }
    #else
    hDLL = dlopen(path, RTLD_LAZY);
    if (!hDLL) {
        throw LmError("Failed to load library");
    }
    #endif
}

void LmCallNative::unCallNative() {
    if (hDLL) {
        #if _WIN64
        FreeLibrary(reinterpret_cast<HMODULE>(hDLL));
        #else
        dlclose(hDLL);
        #endif
    }
}

bool LmCallNative::is_loaded() const {
    return hDLL != nullptr;
}



NativeFuncPtr LmCallNative::get_func(const char *name) const {
    NativeFuncPtr funcptr = nullptr;
    #if _WIN64
        funcptr = reinterpret_cast<NativeFuncPtr>(GetProcAddress(reinterpret_cast<HMODULE>(hDLL), name));
    #else
        funcptr = reinterpret_cast<NativeFuncPtr>(dlsym(hDLL,name));
    #endif

    if (funcptr == nullptr) {

        std::cerr <<
            #if _WIN64
                GetLastError()
            #else
                dlerror()
            #endif
        << '\n';
        throw LmError(std::string("Failed to find function:" + std::string(name)).c_str());
    }
    return funcptr;
}