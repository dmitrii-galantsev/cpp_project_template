/*
 * Convert from uint32_t to string using transform OR alternative methods
 * without iterating over the array.
 */

#include "my_map.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <vector>

void using_transform() {
    std::vector<uint32_t> vec_uint = {1, 2, 3, 5, 8, 13};
    std::vector<std::string> vec_str = {};

    std::transform(std::begin(vec_uint), std::end(vec_uint), std::back_inserter(vec_str),
                   [](uint32_t a) { return std::to_string(a); });

    printf("Size of vec_uint: %lu\n", vec_uint.size());
    for (auto c : vec_uint) {
        printf("[%d]\n", c);
    }

    printf("Size of vec_str: %lu\n", vec_str.size());
    for (auto c : vec_str) {
        printf("[%s]\n", c.c_str());
    }
}

void using_vector_constructor() {
    std::vector<uint32_t> vec_uint = {1, 2, 3, 5, 8, 13};
    // can't construct string from uint32_t!
    // std::vector<std::string> vec_str(vec_uint.begin(), vec_uint.end());
    std::vector<float> vec_str(vec_uint.begin(), vec_uint.end());

    printf("Size of vec_uint: %lu\n", vec_uint.size());
    for (auto c : vec_uint) {
        printf("[%d]\n", c);
    }

    printf("Size of vec_str: %lu\n", vec_str.size());
    for (auto c : vec_str) {
        printf("[%f]\n", c);
    }
}

int my_map() {
    printf("*** Using transform\n");
    using_transform();
    printf("*** Using vector constructor\n");
    using_vector_constructor();
    return 0;
}
