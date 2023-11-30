// Copyright TEST
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iterator>
#include <vector>

int main() {
    // access_types();

    // beef baad dead babe
    const std::vector<uint8_t> my_vec = {0xef, 0xbe, 0xad, 0xba, 0xad, 0xde, 0xbe, 0xba};

    const auto elem_size = static_cast<uint8_t>(std::end(my_vec) - std::begin(my_vec));

    printf("elem_size: [%u]\n", elem_size);

    for (auto i : my_vec) {
        printf("i: [%#04x]\n", i);
    }

    typedef struct hm_t {
        uint64_t a;
        uint64_t b;
    } hm_t;

    // 8
    hm_t store_8;
    auto *store_8_p = (uint8_t *)&store_8;
    std::copy_n(std::begin(my_vec), elem_size, store_8_p);
    printf("store8: a[%#010lx] b[%#010lx]\n", store_8.a, store_8.b);

    // 16
    hm_t store_16;
    auto *store_16_p = (uint16_t *)&store_16;
    std::copy_n(std::begin(my_vec), elem_size, store_16_p);
    printf("store16: a[%#018lx] b[%#018lx]\n", store_16.a, store_16.b);

    // 32
    hm_t store_32;
    auto *store_32_p = (uint32_t *)&store_32;
    // ! This will write outside of the address space!!
    // Error: Triggers ASAN!
    std::copy_n(std::begin(my_vec), elem_size, store_32_p);
    printf("store32: a[%#034lx] b[%#034lx]\n", store_32.a, store_32.b);

    return 0;
}
