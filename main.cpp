#include <fmt/color.h>
#include <fmt/core.h>

#include <vector>

int main() {
    std::vector<std::pair<std::string, int>> scores = {{"Alice", 95}, {"Bob", 87}, {"Charlie", 65}};

    fmt::print(fg(fmt::color::gold), "{:^21}\n", "STUDENT SCORES");
    fmt::print("{:=^21}\n", "");

    for (const auto& [name, score] : scores) {
        auto text_color = score >= 90   ? fg(fmt::color::green)
                          : score >= 70 ? fg(fmt::color::yellow)
                                        : fg(fmt::color::red);

        fmt::print("{:<12} {:>8}\n", name, fmt::styled(std::to_string(score), text_color));
    }

    return 0;
}
