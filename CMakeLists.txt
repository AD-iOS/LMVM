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

file(GLOB_RECURSE lmvm_src vm/*.cpp vm/*/*.cpp)
add_executable(lmvm 
        ${lmvm_src}
)

file(GLOB_RECURSE lmlc_src compiler/*.cpp compiler/*/*.cpp)
add_executable(lmlc
        ${lmlc_src}
)