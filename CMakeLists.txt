cmake_minimum_required(VERSION 4.0)
project(LmInterp)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
    if (ENABLE_FLTO)
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Oi /Ot /Ob2 /Gy /GF /GL")
        set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG")
    else()
        set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Oi /Ot /Ob2 /Gy /GF")
    endif()
else()
    if (ENABLE_FLTO)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math -march=native")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -ffast-math -march=native")
    endif()
endif()

add_executable(lmvm 
        main.cpp
        vm/vm.cpp

        vm/models/Object.cpp
        vm/models/Integer.cpp
        vm/models/Float.cpp
        vm/models/String.cpp

        vm/heap.cpp

        vm/op/OpCode.cpp
        vm/op/Loader.cpp
        
        vm/call/handler.cpp
)

add_executable(lmlc
        ir/main.cpp
        ir/ir_token.cpp
        ir/ir_generator.cpp)