/*
 * I used Bing chatbot (GPT4?) to get the basic code for this
 */

#include "timing.hpp"

#include <chrono>
#include <cstdio>
#include <iostream>

int timing() {
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < 1000; ++i) {
        printf("Hello, world!\n");
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    std::cout << "Execution time for 1000 printf calls: " << duration.count() << " microseconds\n";

    return 0;
}
