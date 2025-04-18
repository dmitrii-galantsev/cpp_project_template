cmake_minimum_required(VERSION 3.14) # Modern CMake version

if(NOT CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
  message(FATAL_ERROR "This toolchain file is intended for Linux hosts only. Current host system is ${CMAKE_HOST_SYSTEM_NAME}")
endif()

find_program(ZIG_EXECUTABLE zig)
if(NOT ZIG_EXECUTABLE)
  message(FATAL_ERROR "Zig executable not found. Please install Zig before using this toolchain file.")
endif()

# Check Zig version
execute_process(
  COMMAND ${ZIG_EXECUTABLE} version
  OUTPUT_VARIABLE ZIG_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Found Zig: ${ZIG_EXECUTABLE} (version: ${ZIG_VERSION})")

# Set up target system
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Configure compilers
set(CMAKE_C_COMPILER "${ZIG_EXECUTABLE}" "cc")
set(CMAKE_CXX_COMPILER "${ZIG_EXECUTABLE}" "c++")

# Set Zig cc/c++ arguments
set(CMAKE_C_FLAGS_INIT "-target x86_64-linux-musl")
set(CMAKE_CXX_FLAGS_INIT "-target x86_64-linux-musl")

# Additional CMake settings for better cross-compilation support
set(CMAKE_CROSSCOMPILING TRUE)
