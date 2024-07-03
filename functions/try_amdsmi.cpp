#include "try_amdsmi.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "amd_smi/amdsmi.h"

#define LOG(X)                                                                               \
    do {                                                                                     \
        if ((X) != AMDSMI_STATUS_SUCCESS) {                                                  \
            std::cout << "AMDSMI ERROR: " << status << " [" << __LINE__ << "]" << std::endl; \
        }                                                                                    \
    } while (0)

amdsmi_status_t get_processor_handle_from_id(uint32_t gpu_id,
                                             amdsmi_processor_handle* processor_handle) {
    uint32_t socket_count;
    uint32_t processor_count;
    auto ret = amdsmi_get_socket_handles(&socket_count, nullptr);
    if (ret != AMDSMI_STATUS_SUCCESS) {
        return ret;
    }
    std::vector<amdsmi_socket_handle> sockets(socket_count);
    std::vector<amdsmi_processor_handle> all_processors{};
    ret = amdsmi_get_socket_handles(&socket_count, sockets.data());
    for (auto& socket : sockets) {
        ret = amdsmi_get_processor_handles(socket, &processor_count, nullptr);
        if (ret != AMDSMI_STATUS_SUCCESS) {
            return ret;
        }
        std::vector<amdsmi_processor_handle> processors(processor_count);
        ret = amdsmi_get_processor_handles(socket, &processor_count, processors.data());
        if (ret != AMDSMI_STATUS_SUCCESS) {
            return ret;
        }

        for (auto& processor : processors) {
            processor_type_t processor_type = {};
            ret = amdsmi_get_processor_type(processor, &processor_type);
            if (processor_type != AMDSMI_PROCESSOR_TYPE_AMD_GPU) {
                std::cout << "Expect AMD_GPU device type!\n";
                return AMDSMI_STATUS_NOT_SUPPORTED;
            }
            all_processors.push_back(processor);
        }
    }

    if (gpu_id >= all_processors.size()) {
        return AMDSMI_STATUS_INPUT_OUT_OF_BOUNDS;
    }

    // Get processor handle from GPU id
    *processor_handle = all_processors[gpu_id];

    return AMDSMI_STATUS_SUCCESS;
}

auto fetch_smi_field(uint32_t gpu_index) {
    // std::shared_ptr<FieldSMIData> smi_data;

    amdsmi_processor_handle processor_handle = {};

    amdsmi_status_t ret = get_processor_handle_from_id(gpu_index, &processor_handle);
    if (ret != AMDSMI_STATUS_SUCCESS) {
        std::cout << "Failed to get processor handle for GPU " << gpu_index << " error: " << ret
                  << "\n";
        return false;
    }

    auto read_gpu_metrics_uint64_t = [&]() {
        amdsmi_gpu_metrics_t gpu_m;
        auto status = amdsmi_get_gpu_metrics_info(processor_handle, &gpu_m);
        std::cout << "Read the gpu metrics:" << status << "\n";
        // if (value->status != AMDSMI_STATUS_SUCCESS) {
        //   return;
        // }

        for (uint32_t i = 0; i < 8; i++) {
            std::cout << "read:  " << i << " [" << gpu_m.xgmi_read_data_acc[i] << "]\n";
            std::cout << "write: " << i << " [" << gpu_m.xgmi_write_data_acc[i] << "]\n";
        }
    };

    read_gpu_metrics_uint64_t();

    return false;
}

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
    fetch_smi_field(0);
    status = amdsmi_shut_down();
    return 0;
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
