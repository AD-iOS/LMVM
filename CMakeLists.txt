cmake_minimum_required(VERSION 3.15)
project(LmInterp)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    if (ENABLE_FLTO)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Oi /Ot /Ob2 /Gy /GF /GL")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Oi /Ot /Ob2 /Gy /GF")
    endif()
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -march=native")
endif()

include_directories(include)

add_executable(lmvm 
        vm/main.cpp
        vm/vm.cpp

        vm/models/Object.cpp
        vm/models/Integer.cpp
        vm/models/Float.cpp
        vm/models/String.cpp

        vm/heap.cpp

        vm/op/OpCode.cpp
        vm/op/Loader.cpp
        
        vm/call/handler.cpp
        vm/models/Types.cpp
        vm/models/Types.h
)

add_executable(lmlc
        compiler/main.cpp
        compiler/ir/ir_token.cpp
        compiler/ir/ir_generator.cpp

        compiler/console.cpp


)

