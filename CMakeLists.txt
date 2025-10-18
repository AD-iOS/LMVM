cmake_minimum_required(VERSION 4.0)
project(LmInterp)

set(CMAKE_CXX_STANDARD 20)

if(MSVC)
set(CMAKE_CXX_FLAGS "/O2 ")
else()
set(CMAKE_CXX_FLAGS "-O3 -flto")
endif()

add_executable(lmvm main.cpp
        vm/vm.cpp

        vm/models/Object.cpp
        vm/models/Integer.cpp
        vm/models/Float.cpp
        vm/models/String.cpp

        vm/heap.cpp

        vm/op/OpCode.cpp
        vm/op/Loader.cpp
        vm/call/handler.cpp
        vm/call/io.cpp
)

add_executable(lmlc
        ir/main.cpp
        ir/ir_token.cpp
        ir/ir_generator.cpp
        ir/ir_generator.h)