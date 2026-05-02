cmake_minimum_required(VERSION 3.15)

project(OsuReplayRecorder)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Os)
    elseif (MSVC)
        add_compile_options(/Os)
    endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)
