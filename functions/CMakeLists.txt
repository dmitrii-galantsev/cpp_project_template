cmake_minimum_required(VERSION 3.11)

set(SOURCES
    functions.hpp functions.cpp
    access_types.hpp access_types.cpp
    bad_std_copy.hpp bad_std_copy.cpp
    type_traits.hpp type_traits.cpp)

add_library(functions_lib STATIC ${SOURCES})
target_compile_options(functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
