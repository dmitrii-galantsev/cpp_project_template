cmake_minimum_required (VERSION 3.11)

set(BINARY functions)
#file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true ${BINARY}.hpp ${BINARY}.cpp)
set(SOURCES ${BINARY}.hpp ${BINARY}.cpp)

add_library(${BINARY}_lib STATIC ${SOURCES})
target_compile_options(${BINARY}_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)


# gtest setup
enable_testing()
add_executable(${BINARY}_test test/${BINARY}_test.cpp)
target_link_libraries(${BINARY}_test PUBLIC ${BINARY}_lib gtest_main)
#target_link_libraries(${BINARY}_test ${BINARY}_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(${BINARY}_test)
