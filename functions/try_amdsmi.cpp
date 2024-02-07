#include "try_amdsmi.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

#include "amd_smi/amdsmi.h"

#define LOG(X)                                                                               \
    do {                                                                                     \
        if ((X) != AMDSMI_STATUS_SUCCESS) {                                                  \
            std::cout << "AMDSMI ERROR: " << status << " [" << __LINE__ << "]" << std::endl; \
        }                                                                                    \
    } while (0)

int try_amdsmi() {
    amdsmi_status_t status = AMDSMI_STATUS_UNKNOWN_ERROR;
    status = amdsmi_init(AMDSMI_INIT_AMD_GPUS);
    LOG(status);
    uint32_t socket_count = 0;
    status = amdsmi_get_socket_handles(&socket_count, nullptr);
    LOG(status);
    std::cout << "Socket count: " << socket_count << std::endl;

    std::vector<amdsmi_socket_handle> sockets(socket_count);
    status = amdsmi_get_socket_handles(&socket_count, sockets.data());
    LOG(status);
    for (auto& socket : sockets) {
        char name[50];
        status = amdsmi_get_socket_info(socket, 50, name);
        std::cout << "Socket name: " << name << "\n";

        uint32_t processor_count = 0;
        status = amdsmi_get_processor_handles(socket, &processor_count, nullptr);
        LOG(status);
        std::cout << "Processor count: " << processor_count << "\n";

        std::vector<amdsmi_processor_handle> processors(processor_count);
        status = amdsmi_get_processor_handles(socket, &processor_count, processors.data());

        for (auto& processor : processors) {
            processor_type_t processor_type = AMDSMI_PROCESSOR_TYPE_UNKNOWN;
            status = amdsmi_get_processor_type(processor, &processor_type);
            std::cout << "Processor type: " << processor_type << "\n";
            amdsmi_board_info_t board_info;
            status = amdsmi_get_gpu_board_info(processor, &board_info);
            std::cout << "Board name: " << board_info.model_number << "\n";
            amdsmi_asic_info_t info;
            status = amdsmi_get_gpu_asic_info(processor, &info);
            std::cout << "Asic info: " << info.market_name << "\n";
        }
    }

    status = amdsmi_shut_down();
    LOG(status);

    return 0;
}
