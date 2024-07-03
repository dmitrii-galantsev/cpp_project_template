cmake_minimum_required(VERSION 3.11)

set(SOURCES
    functions.cpp
    access_types.cpp
    bad_std_copy.cpp
    #smart_pointers.cpp
    try_amdsmi.cpp
    try_scandir.cpp
    timing.cpp)

set(ROCM_DIR "/opt/rocm/" CACHE PATH "ROCm directory")
find_package(SMI
    NAMES amd_smi
    HINTS ${ROCM_DIR}/lib/cmake
    CONFIGURE REQUIRED)
set(SMI_INC_DIR "${AMD_SMI_INCLUDE_DIR}" CACHE INTERNAL "AMD SMI include directory.")
set(SMI_LIB_DIR "${AMD_SMI_LIB_DIR}" CACHE INTERNAL "AMD SMI library directory.")

if(NOT EXISTS "${SMI_INC_DIR}" OR NOT EXISTS "${SMI_LIB_DIR}")
    message(FATAL_ERROR "amd_smi not found in ${SMI_INC_DIR}. Please
    make sure amd_smi is installed and present in ${SMI_INC_DIR}.")
endif()

add_library(functions_lib STATIC ${SOURCES})
target_include_directories(functions_lib PUBLIC ${SMI_INC_DIR})
target_link_libraries(functions_lib amd_smi)
target_compile_options(functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
