cmake_minimum_required(VERSION 3.21)

set(SOURCES
    functions.hpp functions.cpp
    access_types.hpp access_types.cpp
    bad_std_copy.hpp bad_std_copy.cpp
    #smart_pointers.hpp smart_pointers.cpp
    try_amdsmi_mutex.cpp try_amdsmi_mutex.hpp
    try_amdsmi.cpp try_amdsmi.hpp
    try_scandir.cpp try_scandir.hpp
    timing.cpp timing.hpp)

set(ROCM_DIR "/opt/rocm/" CACHE PATH "ROCm directory")
find_package(amd_smi
    NAMES amd_smi
    HINTS ${ROCM_DIR}/lib/cmake
    CONFIG REQUIRED)
set(SMI_INC_DIR "${AMD_SMI_INCLUDE_DIR}" CACHE INTERNAL "AMD SMI include directory.")
set(SMI_LIB_DIR "${AMD_SMI_LIB_DIR}" CACHE INTERNAL "AMD SMI library directory.")

add_definitions("-DENABLE_ESMI_LIB=1")

if(NOT EXISTS "${SMI_INC_DIR}" OR NOT EXISTS "${SMI_LIB_DIR}")
    message(FATAL_ERROR "amd_smi not found in ${SMI_INC_DIR}. Please
    make sure amd_smi is installed and present in ${SMI_INC_DIR}.")
endif()
add_library(functions_lib STATIC ${SOURCES})
#add_library(functions_lib SHARED ${SOURCES})
target_include_directories(functions_lib PUBLIC ${SMI_INC_DIR})
target_link_libraries(functions_lib amd_smi)
target_compile_options(functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
install(TARGETS functions_lib)
