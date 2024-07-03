#include "my_rocprofiler.hpp"

#include <cstdio>

#include "hsa/hsa.h"
#include "rocprofiler/rocprofiler.h"

int my_rocprofiler() {
    hsa_status_t status = HSA_STATUS_SUCCESS;
    status = hsa_init();
    printf("Init Status: %d\n", status);
    status = hsa_shut_down();
    printf("Shutdown Status: %d\n", status);
    status = hsa_shut_down();
    printf("Shutdown Status: %d\n", status);
    return 0;
}
