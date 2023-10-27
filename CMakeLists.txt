cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/"
    CACHE INTERNAL "Default module path.")

option(CMAKE_VERBOSE_MAKEFILE "Enable verbose output" ON)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export compile commands for linters and autocompleters" ON)

project(main LANGUAGES CXX)

option(BUILD_TESTS "Build test suite" OFF)
option(ADDRESS_SANITIZER "Enable address sanitizer" OFF)

set(CMAKE_CXX_STANDARD
    17
    CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED
    ON
    CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS
    OFF
    CACHE BOOL "")

if(CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.4.0)
    message("Compiler version is " ${CMAKE_CXX_COMPILER_VERSION})
    message(FATAL_ERROR "Require at least gcc-5.4.0")
endif()

# Clang does not set the build-id
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--build-id=sha1")
endif()

## Add address sanitizer
if(${ADDRESS_SANITIZER})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -shared-libasan")
    message(STATUS "ADDRESS_SANITIZE: CMAKE_CXX_FLAGS=: ${CMAKE_CXX_FLAGS}")
    message(STATUS "ADDRESS_SANITIZE: CMAKE_EXE_LINKER_FLAGS=: ${CMAKE_EXE_LINKER_FLAGS}")
endif()

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
cmake_print_properties(TARGETS main PROPERTIES INCLUDE_DIRECTORIES LINK_LIBRARIES)
cmake_print_properties(SOURCES main PROPERTIES LOCATION)

# trace a run
# trace CMakeLists.txt line-by-line:
#       cmake -B build --trace-source=CMakeLists.txt
# add '--trace-expand' to expand the variables
