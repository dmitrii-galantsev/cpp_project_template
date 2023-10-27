// Copyright TEST

#include <utility>

#include "functions/functions.hpp"
template <typename F, typename... Args>
void rsmi_wrapper(F&& f, Args&&... args) {
    auto rstatus = std::forward<F>(f)(123, std::forward<Args>(args)...);
}

int main() {
    run_functions();

    return 0;
}
