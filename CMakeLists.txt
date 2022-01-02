cmake_minimum_required (VERSION 3.11)

project(basic-cpp-features
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# fetch google test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#add_executable(ftxui-starter src/main.cpp)
add_subdirectory(functions)

set(BINARY main)
set(SOURCES ${BINARY}.cpp)
add_executable(${BINARY} ${SOURCES})
target_link_libraries(${BINARY} functions_lib)
