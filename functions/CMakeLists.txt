cmake_minimum_required(VERSION 3.11)

set(SOURCES
    functions.cpp
    access_types.cpp
    bad_std_copy.cpp
    smart_pointers.cpp
    try_scandir.cpp
    timing.cpp)

add_library(functions_lib STATIC ${SOURCES})
target_compile_options(functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
