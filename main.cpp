#include <fmt/chrono.h>
#include <fmt/core.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

namespace fs = std::filesystem;

const fs::path drm_path = "/sys/class/drm";
std::vector<fs::path> gpu_metric_files = {};
typedef struct {
    uint8_t major;
    uint8_t minor;
    uint16_t gfx_activity;
} gpu_metrics_t;

template <>
struct fmt::formatter<gpu_metrics_t> {
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const gpu_metrics_t& p, FormatContext& ctx) const {
        return fmt::format_to(ctx.out(),
                              "├── GPU Metrics version: {}.{}\n"
                              "└── GFX Activity: {:#06x} = {:d}\n",
                              p.major, p.minor, p.gfx_activity, p.gfx_activity);
    }
};

void find_gpu_metric() {
    if (!fs::exists(drm_path) || !fs::is_directory(drm_path)) {
        fmt::print("DRM path does not exist or is not a directory.\n");
        return;
    }

    for (const auto& entry : fs::directory_iterator(drm_path)) {
        if (!entry.is_directory()) continue;

        auto path = entry.path();

        std::regex card_pattern("^card[0-9]+$");

        // cant find /sys/class/drm/card[0-9]+/
        if (!std::regex_match(path.filename().string(), card_pattern)) {
            continue;
        }

        // cant find /sys/class/drm/card*/device
        if (!fs::exists(path / "device")) {
            continue;
        }

        auto device_path = fs::path(path / "device");
        for (const auto& file : fs::directory_iterator(device_path)) {
            if (file.path().filename() == "gpu_metrics") {
                gpu_metric_files.emplace_back(file.path());
            }
        }
    }
}

void print_version(fs::path& path) {
    gpu_metrics_t metric;

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        fmt::print("Failed to open file: {}\n", path.string());
        return;
    }

    file.seekg(0, std::ios::end);
    auto filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    if (filesize < 10) {
        fmt::print("File is too small to read version: {}\n", path.string());
        return;
    }

    std::vector<std::byte> buffer(filesize);
    file.read(reinterpret_cast<char*>(buffer.data()), filesize);

    size_t pos = 0x02;
    metric.major = static_cast<uint8_t>(buffer.at(pos));
    metric.minor = static_cast<uint8_t>(buffer.at(pos + 1));

    if (metric.minor >= 0 && metric.minor <= 3) {
        pos = 0x10;
    } else if (metric.minor >= 4) {
        pos = 0x0c;
    } else {
        fmt::print("Unknown minor version: {}\n", metric.minor);
        return;
    }
    metric.gfx_activity =
        (static_cast<uint16_t>(buffer.at(pos)) | static_cast<uint16_t>(buffer.at(pos + 1)) << 8);

    // fmt::print("[{:04x}] = {:02}.{:02}\n", pos, metrics.major, metrics.minor);

    fmt::print("{}\n", metric);
}

int main() {
    int status = 0;

    fmt::print("Hello, World!\n");

    auto now = std::chrono::system_clock::now();
    fmt::print("Date and time: {}\n", now);
    fmt::print("Time: {:%H:%M}\n", now);

    fmt::print("... Looking for gpu_metrics...\n");
    find_gpu_metric();

    sort(gpu_metric_files.begin(), gpu_metric_files.end());

    for (auto& file : gpu_metric_files) {
        fmt::print("{}\n", file.string());
        print_version(file);
    }

    return status;
}
