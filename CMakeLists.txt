cmake_minimum_required(VERSION 3.20)

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/"
    CACHE INTERNAL "Default module path.")

option(CMAKE_VERBOSE_MAKEFILE "Enable verbose output" ON)
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export compile commands for linters and autocompleters" ON)

set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build to perform: Debug, Release, RelWithDebInfo, MinSizeRel")
# Set the possible values of build type
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")

option(BUILD_WITH_ZIG "Enable cross compilation with zig" ON)
if(BUILD_WITH_ZIG)
    set(BUILD_SHARED_LIBS OFF)
    include(zig)
endif()

## Set compile flags
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wall -Wextra -m64 -msse -msse2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -DDEBUG" CACHE STRING "Flags for Debug builds")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -s -DNDEBUG" CACHE STRING "Flags for Release builds")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g -DNDEBUG" CACHE STRING "Flags for RelWithDebInfo builds")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG" CACHE STRING "Flags for MinSizeRel builds")

## Security breach mitigation flags
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -DFORTIFY_SOURCE=2 -fstack-protector-all -Wcast-align")

project(main LANGUAGES CXX)

set(CMAKE_CXX_STANDARD
    17
    CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED
    ON
    CACHE BOOL "")
set(CMAKE_CXX_EXTENSIONS
    OFF
    CACHE BOOL "")

# Include CMAKE_INSTALL_* variables
# this must go after project()
include(GNUInstallDirs)
# search for packages
include(FindPkgConfig)

include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 11.1.4  # Use a specific version tag
)
FetchContent_MakeAvailable(fmt)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE fmt::fmt)
