cmake_minimum_required(VERSION 3.11)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/"
    CACHE INTERNAL "Default module path.")

option(CMAKE_VERBOSE_MAKEFILE "Enable verbose output" ON)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export compile commands for linters and autocompleters" ON)

project(main LANGUAGES CXX)

option(BUILD_TESTS "Build test suite" OFF)

set(CMAKE_CXX_STANDARD
    11
    CACHE STRING "The C++ standard to use")

# Include CMAKE_INSTALL_* variables
# this must go after project()
include(GNUInstallDirs)

add_subdirectory(functions)
add_executable(main main.cpp)
target_link_libraries(main functions_lib)

if(BUILD_TESTS)
    add_subdirectory("tests")
endif()

### Useful debug stuff

### TODO: DELETE
# print variables
include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_STANDARD)
# function to print vars
function(print)
    message("--")
    cmake_print_variables(${ARGN})
endfunction()
function(print_fatal)
    print(${ARGN})
    message(FATAL_ERROR "--- last print")
endfunction()
### TODO: DELETE

# print properties
cmake_print_properties(TARGETS main
    PROPERTIES
    INCLUDE_DIRECTORIES
    LINK_LIBRARIES)
cmake_print_properties(SOURCES main
    PROPERTIES
    LOCATION)

# trace a run
# trace CMakeLists.txt line-by-line:
#       cmake -B build --trace-source=CMakeLists.txt
# add '--trace-expand' to expand the variables

### Finding packages

# add package search paths
#set(CMAKE_PREFIX_PATH
#    ${CMAKE_PREFIX_PATH}
#    /usr/local
#    )
#set(CMAKE_LIBRARY_PATH
#    ${CMAKE_LIBRARY_PATH}
#    /usr/lib64
#    /usr/lib/x86_64-linux-gnu
#    )

# find lib
#find_library(LIB_CAP
#    NAMES cap
#    REQUIRED)

# find package (preferred!)
#find_package(protobuf
#    HINTS ${GRPC_ROOT}
#    CONFIG REQUIRED)
#find_package(gRPC ${GRPC_DESIRED_VERSION}
#    HINTS ${GRPC_ROOT}
#    CONFIG REQUIRED)

