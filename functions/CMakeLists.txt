cmake_minimum_required(VERSION 3.11)

set(SOURCES
    functions.cpp
    access_types.cpp
    bad_std_copy.cpp
    try_scandir.cpp
    timing.cpp)

find_package(Boost 1.36.0)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(SOURCES ${SOURCES} smart_pointers.cpp)
endif()

add_library(functions_lib STATIC ${SOURCES})
target_compile_options(functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
