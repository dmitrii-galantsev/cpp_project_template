cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/"
    CACHE INTERNAL "Default module path.")

option(CMAKE_VERBOSE_MAKEFILE "Enable verbose output" ON)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export compile commands for linters and autocompleters" ON)

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build to perform: Debug, Release, RelWithDebInfo, MinSizeRel")
# Set the possible values of build type
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

# Set compile flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -m64 -msse -msse2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -DDEBUG" CACHE STRING "Flags for Debug builds")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -DNDEBUG" CACHE STRING "Flags for Release builds")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DNDEBUG" CACHE STRING "Flags for RelWithDebInfo builds")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG" CACHE STRING "Flags for MinSizeRel builds")

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

# Default libdir to "lib", this skips GNUInstallDirs from trying to take a guess if it's unset:
set(CMAKE_INSTALL_LIBDIR
    "lib"
    CACHE STRING "Library install directory")

# Let libraries be found using rpath
set(CMAKE_SHARED_LINKER_FLAGS_INIT
    "-Wl,--enable-new-dtags,--rpath,\$ORIGIN"
    CACHE STRING "Linker flags INIT")
set(CMAKE_EXE_LINKER_FLAGS_INIT
    "-Wl,--enable-new-dtags,--rpath,\$ORIGIN:\$ORIGIN/../lib"
    CACHE STRING "Linker flags INIT")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH
    TRUE
    CACHE BOOL "Use link path for install RPATH")
set(CMAKE_BUILD_RPATH_USE_ORIGIN
    TRUE
    CACHE BOOL "Set all RPATH to be ORIGIN-based")

# Clang does not set the build-id
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_SHARED_LINKER_FLAGS_INIT "${CMAKE_SHARED_LINKER_FLAGS_INIT},--build-id=sha1")
    set(CMAKE_EXE_LINKER_FLAGS_INIT "${CMAKE_EXE_LINKER_FLAGS_INIT},-Wl,--build-id=sha1")
endif()

## Address Sanitize Flag
if(NOT ${ADDRESS_SANITIZER})
    ## Security breach mitigation flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFORTIFY_SOURCE=2 -fstack-protector-all -Wcast-align")
    ## More security breach mitigation flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-z,noexecstack -Wl,-znoexecheap -Wl,-z,relro ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wtrampolines -Wl,-z,now")
endif()

# Include CMAKE_INSTALL_* variables
# this must go after project()
include(GNUInstallDirs)
# search for packages
include(FindPkgConfig)

#pkg_check_modules(DRM REQUIRED libdrm)

add_executable(main main.cpp)
set_target_properties(main PROPERTIES INSTALL_RPATH "\$ORIGIN/../lib")
install(TARGETS main)

if(BUILD_TESTS)
    add_subdirectory("tests")
endif()

### Useful debug stuff

include(CMakePrintHelpers)
# Usage: print(VARIABLE[s])
# Examples:
#   print(CMAKE_CXX_STANDARD)
#   -- CMAKE_CXX_STANDARD="17"
#   print(CMAKE_CXX_STANDARD CMAKE_CXX_FLAGS)
#   -- CMAKE_CXX_STANDARD="17" ; CMAKE_CXX_FLAGS=" -Wall -Wextra -m64 -msse -msse2 -ggdb -O0 -DDEBUG"
function(print)
    message("--")
    cmake_print_variables(${ARGN})
endfunction()
function(print_fatal)
    print(${ARGN})
    message(FATAL_ERROR "--- last print")
endfunction()

# print properties
cmake_print_properties(TARGETS main PROPERTIES INCLUDE_DIRECTORIES LINK_LIBRARIES)
cmake_print_properties(SOURCES main PROPERTIES LOCATION)

# print all variables starting with "main"
get_cmake_property(_var_names VARIABLES)
list(SORT _var_names)
foreach(_var_name ${_var_names})
    if(_var_name MATCHES "^main")
        message(STATUS "${_var_name}=${${_var_name}}")
    endif()
endforeach()

# trace a run
# trace CMakeLists.txt line-by-line:
#       cmake -B build --trace-source=CMakeLists.txt
# add '--trace-expand' to expand the variables
