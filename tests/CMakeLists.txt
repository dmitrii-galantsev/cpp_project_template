# Required Defines first:
option(INSTALL_GTEST "Install GTest (only useful if GTest is not already installed)" OFF)

# Try to find googletest
find_package(GTest 1.12.0)

# GTest_FOUND is set to TRUE if ANY version is found
# GTest_VERSION is set if 1.11.0 or newer version is found
if(NOT GTest_FOUND STREQUAL "TRUE" OR NOT DEFINED GTest_VERSION)
    # Google Test wasn't found. Download and compile ourselves
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.0)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()

add_executable(functions_test functions_test.cpp)
target_include_directories(functions_test PUBLIC ../functions)
target_link_libraries(functions_test PUBLIC functions_lib GTest::gtest_main)

gtest_discover_tests(functions_test)
