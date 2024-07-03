cmake_minimum_required(VERSION 3.11)

set(ROCM_DIR "/opt/rocm/" CACHE PATH "ROCm directory")

find_package(hsa-runtime64
    REQUIRED
    HINTS ${ROCM_DIR}/lib/cmake)

include(Findrocprofiler)

set(SOURCES
    my_rocprofiler.cpp)

add_library(hsa_functions_lib SHARED ${SOURCES})
target_compile_options(hsa_functions_lib PUBLIC -Wall -pedantic -Wextra -Wconversion -Wsign-conversion)
target_link_libraries(hsa_functions_lib hsa-runtime64::hsa-runtime64 rocprofiler::rocprofiler)
install(TARGETS hsa_functions_lib)
