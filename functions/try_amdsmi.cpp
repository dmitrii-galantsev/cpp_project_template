#include "try_amdsmi.hpp"

#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

#include "amd_smi/amdsmi.h"

#define LOG(X)                                                                          \
    do {                                                                                \
        if ((X) != AMDSMI_STATUS_SUCCESS) {                                             \
            std::cout << __FUNCTION__ << ":" << __LINE__ << " AMDSMI ERROR: " << status \
                      << std::endl;                                                     \
        }                                                                               \
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
    status = amdsmi_init(AMDSMI_INIT_AMD_CPUS | AMDSMI_INIT_AMD_GPUS);
    LOG(status);

    uint32_t socket_count = 0;
    status = amdsmi_get_socket_handles(&socket_count, nullptr);
    LOG(status);
    std::cout << "Socket count: " << socket_count << std::endl;
    std::vector<amdsmi_socket_handle> sockets(socket_count);
    status = amdsmi_get_socket_handles(&socket_count, sockets.data());
    LOG(status);

    // return 0;
    for (auto& socket : sockets) {
        std::cout << "Socket handle: " << socket << "\n";

        uint32_t gpus_per_soc = 0;
        status = amdsmi_get_processor_handles_by_type(socket, AMDSMI_PROCESSOR_TYPE_AMD_GPU,
                                                      nullptr, &gpus_per_soc);
        LOG(status);
        std::cout << "  GPU per soc count: " << gpus_per_soc << "\n";
        std::vector<amdsmi_processor_handle> gpus(gpus_per_soc);
        status = amdsmi_get_processor_handles_by_type(socket, AMDSMI_PROCESSOR_TYPE_AMD_GPU,
                                                      gpus.data(), &gpus_per_soc);
        LOG(status);

        uint32_t cpus_per_soc = 0;
        status = amdsmi_get_processor_handles_by_type(socket, AMDSMI_PROCESSOR_TYPE_AMD_CPU,
                                                      nullptr, &cpus_per_soc);
        LOG(status);
        std::cout << "  CPU per soc count: " << cpus_per_soc << "\n";
        std::vector<amdsmi_processor_handle> cpus(cpus_per_soc);
        status = amdsmi_get_processor_handles_by_type(socket, AMDSMI_PROCESSOR_TYPE_AMD_CPU,
                                                      cpus.data(), &cpus_per_soc);
        LOG(status);

        uint32_t cores_per_soc = 0;
        status = amdsmi_get_processor_handles_by_type(socket, AMDSMI_PROCESSOR_TYPE_AMD_CPU_CORE,
                                                      nullptr, &cores_per_soc);
        LOG(status);
        std::cout << "  CPU_CORE per soc count: " << cores_per_soc << "\n";
        std::vector<amdsmi_processor_handle> cores(cores_per_soc);
        status = amdsmi_get_processor_handles_by_type(socket, AMDSMI_PROCESSOR_TYPE_AMD_CPU_CORE,
                                                      cores.data(), &cores_per_soc);
        LOG(status);
        continue;

        // GPUs
        for (auto& gpu : gpus) {
            std::cout << "  GPU handle: " << gpu << "\n";
            char gpu_name[AMDSMI_MAX_STRING_LENGTH] = {};
            status = amdsmi_get_gpu_vendor_name(gpu, gpu_name, AMDSMI_MAX_STRING_LENGTH);
            LOG(status);
            std::cout << "    GPU vendor name: " << gpu_name << "\n";
        }

        // CPUs
        for (auto& cpu : cpus) {
            std::cout << "  CPU handle: " << cpu << "\n";
            amdsmi_cpu_info_t cpu_info = {};
            status = amdsmi_get_cpu_model_name(cpu, &cpu_info);
            LOG(status);
            std::cout << "    CPU name: " << cpu_info.model_name << "\n";
        }

        // CORES
        for (auto& core : cores) {
            std::cout << "  Core handle: " << core << "\n";
            uint32_t boostlimit = 0;
            status = amdsmi_get_cpu_core_boostlimit(core, &boostlimit);
            LOG(status);
            if (status == AMDSMI_STATUS_SUCCESS)
                std::cout << "    Boostlimit: " << boostlimit << "\n";

            uint32_t freq = 0;
            status = amdsmi_get_cpu_core_current_freq_limit(core, &freq);
            LOG(status);
            if (status == AMDSMI_STATUS_SUCCESS)
                std::cout << "    Current frequency limit: " << freq << " MHz\n";

            uint64_t energy = 0;
            status = amdsmi_get_cpu_core_energy(core, &energy);
            // LOG(status);
            if (status == AMDSMI_STATUS_SUCCESS) std::cout << "    Energy: " << energy << " nJ\n";
        }
    }

    status = amdsmi_shut_down();
    LOG(status);

    return 0;
}
