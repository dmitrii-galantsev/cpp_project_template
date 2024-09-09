cmake_minimum_required(VERSION 3.11)

set(SOURCES
    functions.cpp)

add_library(functions_lib SHARED ${SOURCES})
target_compile_options(functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
install(TARGETS functions_lib)
