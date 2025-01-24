#include <rocprofiler-sdk/fwd.h>
#include <rocprofiler-sdk/registration.h>
#include <rocprofiler-sdk/rocprofiler.h>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

template <typename Callable>
void RocprofilerCall(Callable&& callable, const std::string& msg, const char* file, int line) {
    auto result = callable();
    if (result != ROCPROFILER_STATUS_SUCCESS) {
        std::string status_msg = rocprofiler_get_status_string(result);
        std::cout << "[CALL][" << file << ":" << line << "] " << msg << " failed with error code "
                  << result << ": " << status_msg << std::endl;
        std::stringstream errmsg{};
        errmsg << "[CALL][" << file << ":" << line << "] " << msg << " failure (" << status_msg
               << ")";
        throw std::runtime_error(errmsg.str());
    }
}

std::vector<rocprofiler_agent_v0_t> get_available_agents() {
    std::vector<rocprofiler_agent_v0_t> agents;
    rocprofiler_query_available_agents_cb_t iterate_cb = [](rocprofiler_agent_version_t agents_ver,
                                                            const void** agents_arr,
                                                            size_t num_agents, void* udata) {
        if (agents_ver != ROCPROFILER_AGENT_INFO_VERSION_0)
            throw std::runtime_error{"unexpected rocprofiler agent version"};
        auto* agents_v = static_cast<std::vector<rocprofiler_agent_v0_t>*>(udata);
        for (size_t i = 0; i < num_agents; ++i) {
            const auto* rocp_agent = static_cast<const rocprofiler_agent_v0_t*>(agents_arr[i]);
            if (rocp_agent->type == ROCPROFILER_AGENT_TYPE_GPU) agents_v->emplace_back(*rocp_agent);
        }
        return ROCPROFILER_STATUS_SUCCESS;
    };

    RocprofilerCall(
        [&]() {
            return rocprofiler_query_available_agents(
                ROCPROFILER_AGENT_INFO_VERSION_0, iterate_cb, sizeof(rocprofiler_agent_t),
                const_cast<void*>(static_cast<const void*>(&agents)));
        },
        "query available agents", __FILE__, __LINE__);
    return agents;
}

int main() {
    auto agents = get_available_agents();
    for (auto& agent : agents) {
        std::cout << "name: " << agent.name << "\n"
                  << "device_id: " << agent.device_id << "\n"
                  << "cu_count: " << agent.cu_count << "\n"
                  << "simd_count: " << agent.simd_count << "\n"
                  << "num_xcc: " << agent.num_xcc << "\n"
                  << "size: " << agent.size << "\n"
                  << "max_engine_clk_fcompute: " << agent.max_engine_clk_fcompute << "\n"
                  << "cu_per_engine: " << agent.cu_per_engine << "\n"
                  << "simd_per_cu: " << agent.simd_per_cu << "\n"
                  << "max_waves_per_simd: " << agent.max_waves_per_simd << "\n"
                  << "max_waves_per_cu: " << agent.max_waves_per_cu << "\n";
        std::cout << "\n";
    }

    return 0;
}
