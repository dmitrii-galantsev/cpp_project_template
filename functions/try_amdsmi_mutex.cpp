#include "try_amdsmi_mutex.hpp"

#include <chrono>
#include <cstring>
#include <iostream>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <vector>

#include "amd_smi/amdsmi.h"  // @manual=fbsource//third-party/rocm:amd_smi-lazy
// Default number of threads if not specified
constexpr int DEFAULT_THREAD_COUNT = 8;
// Mutex for thread-safe console output
std::mutex cout_mutex;
// Forward declaration
void amdsmi_thread_func(int thread_id);
// Helper function to print AMDSMI errors
void print_amdsmi_error(const char* function_name, amdsmi_status_t status, int thread_id) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cerr << " [" << thread_id << "]"
              << "Error in " << function_name << ": " << status << std::endl;
}
// Thread function implementation
void amdsmi_thread_func(int thread_id) {
    // Initialize AMDSMI
    auto status = amdsmi_init(AMDSMI_INIT_AMD_GPUS);
    if (status != AMDSMI_STATUS_SUCCESS) {
        print_amdsmi_error("amdsmi_init", status, thread_id);
    }
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread " << thread_id << " initialized " << std::endl;
    }
    // Random sleep to avoid all threads starting at the same time
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sleep_dist(100, 2000);
    int sleep_time = sleep_dist(gen);
    // Get number of devices
    uint32_t num_devices = 2;
    std::vector<amdsmi_socket_handle> socket_handles(num_devices);
    status = amdsmi_get_socket_handles(&num_devices, socket_handles.data());
    if (status != AMDSMI_STATUS_SUCCESS) {
        print_amdsmi_error("amdsmi_get_gpu_asic_info", status, thread_id);
    }
    std::vector<amdsmi_processor_handle> processor_handles;
    for (uint32_t device_id = 0; device_id < num_devices; device_id++) {
        uint32_t num_partitions = 1;
        std::vector<amdsmi_processor_handle> temp(num_partitions);
        status =
            amdsmi_get_processor_handles(socket_handles[device_id], &num_partitions, temp.data());
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_processor_handles", status, thread_id);
            continue;
        }
        processor_handles.insert(processor_handles.end(), temp.begin(), temp.end());
    }
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread " << thread_id << " starting (sleeping for " << sleep_time << "ms)..."
                  << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));
    // Iterate through each device
    for (const auto& handle : processor_handles) {
        // Get ASIC info
        amdsmi_asic_info_t asic_info;
        status = amdsmi_get_gpu_asic_info(handle, &asic_info);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_asic_info", status, thread_id);
        }
        // Get board info
        amdsmi_board_info_t board_info;
        status = amdsmi_get_gpu_board_info(handle, &board_info);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_board_info", status, thread_id);
        }
        // Get GPU activity
        amdsmi_engine_usage_t activity;
        status = amdsmi_get_gpu_activity(handle, &activity);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_activity", status, thread_id);
        }
        // Get VBIOS info
        amdsmi_vbios_info_t vbios_info;
        status = amdsmi_get_gpu_vbios_info(handle, &vbios_info);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_vbios_info", status, thread_id);
        }
        // Get bad page info
        amdsmi_retired_page_record_t* bad_pages = nullptr;
        uint32_t num_pages = 0;
        status = amdsmi_get_gpu_bad_page_info(handle, &num_pages, bad_pages);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_bad_page_info", status, thread_id);
        }
        // Get ECC count
        amdsmi_error_count_t ecc_count;
        status = amdsmi_get_gpu_total_ecc_count(handle, &ecc_count);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_total_ecc_count", status, thread_id);
        }
        // Get frequency information
        amdsmi_frequencies_t freq_info;
        status = amdsmi_get_clk_freq(handle, AMDSMI_CLK_TYPE_GFX, &freq_info);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_clk_freq", status, thread_id);
        }
        // Get GPU metrics info
        amdsmi_gpu_metrics_t metrics;
        status = amdsmi_get_gpu_metrics_info(handle, &metrics);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_metrics_info", status, thread_id);
        }
        // Get GPU KFD info
        amdsmi_kfd_info_t kfd_info;
        status = amdsmi_get_gpu_kfd_info(handle, &kfd_info);
        if (status != AMDSMI_STATUS_SUCCESS) {
            print_amdsmi_error("amdsmi_get_gpu_kfd_info", status, thread_id);
        }
    }

    // Each thread that calls amdsmi_init() should also call amdsmi_shut_down()
    status = amdsmi_shut_down();
    if (status != AMDSMI_STATUS_SUCCESS) {
        print_amdsmi_error("amdsmi_shut_down", status, thread_id);
    } else {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Thread " << thread_id << " shut down AMDSMI successfully" << std::endl;
    }
}

int try_amdsmi_mutex(int argc, char* argv[]) {
    int thread_count = DEFAULT_THREAD_COUNT;
    // Parse command line arguments
    if (argc > 1) {
        try {
            thread_count = std::stoi(argv[1]);
            if (thread_count <= 0) {
                std::cerr << "Thread count must be positive. Using default: "
                          << DEFAULT_THREAD_COUNT << std::endl;
                thread_count = DEFAULT_THREAD_COUNT;
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid thread count. Using default: " << DEFAULT_THREAD_COUNT
                      << std::endl;
            thread_count = DEFAULT_THREAD_COUNT;
        }
    }
    int thread_id = 12345;  // Placeholder for thread ID, can be set per thread if needed
    std::cout << "Starting AMDSMI demo with " << thread_count << " threads..." << std::endl;
    // Create threads
    std::vector<std::thread> threads;
    threads.reserve(thread_count);
    for (int i = 0; i < thread_count; i++) {
        threads.emplace_back(amdsmi_thread_func, i);
    }
    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All threads completed successfully." << std::endl;
    return 0;
}
