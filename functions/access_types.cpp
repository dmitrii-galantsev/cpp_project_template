#include "access_types.hpp"

#include <cstddef>
#include <iostream>
#include <utility>

template <typename T>
bool is_nullptr(T *value) {
    return value == nullptr;
}

template <typename T>
bool is_nullptr(T) {
    return false;
}

template <typename T, typename... Args>
bool is_nullptr(T value, Args &&...args) {
    // std::cout << "[" << value << "]\n";
    return is_nullptr(value) || is_nullptr(std::forward<Args>(args)...);
}

template <typename... Args>
void arg_wrapper(Args &&...args) {
    std::cout << "wrapper of [" << sizeof...(Args) << "] arguments\n";

    if (is_nullptr(0, std::forward<Args>(args)...)) {
        std::cout << "NULLPTR\n";
    } else {
        std::cout << "NO NULLPTR\n";
    }
}

template <typename T>
constexpr T bypass_cast(std::nullptr_t) {
    std::cout << "FUCK\n";
    return nullptr;
}

template <typename T, typename U>
constexpr T bypass_cast(U value) {
    return reinterpret_cast<T>(value);
}

typedef struct {
    int a;
    float b;
} cool_t;

typedef struct {
    int a;
    int b;
} lame_t;

void printme(cool_t *x) {
    if (x == nullptr) {
        std::cout << "NULLPTR\n";
        return;
    }
    std::cout << x->a << "..." << x->b << std::endl;
}

void printme(int *x) {
    if (x == nullptr) {
        std::cout << "NULLPTR\n";
        return;
    }
    std::cout << *x << std::endl;
}

void printme(float *x) {
    if (x == nullptr) {
        std::cout << "NULLPTR\n";
        return;
    }
    std::cout << *x << std::endl;
}

int access_types() {
    int *k = nullptr;
    cool_t cool = {69, 420.1337};
    lame_t lame = {1, 2};
    lame_t *lame_null = nullptr;

    // arg_wrapper(1);
    // arg_wrapper(2, "");
    // arg_wrapper(3, k, 1);
    // arg_wrapper(4, 0.123, k, std::nullptr_t());
    // arg_wrapper(5, 5, 0.123, k, 5);

    printme(k);
    printme(bypass_cast<float *>(k));
    printme(reinterpret_cast<float *>(k));
    printme(bypass_cast<float *>(nullptr));  // legal
    // printme(reinterpret_cast<float *>(nullptr)); // illegal!
    int j = 10;
    printme(&j);
    printme(bypass_cast<float *>(&j));
    printme(reinterpret_cast<float *>(&j));
    printme(bypass_cast<cool_t *>(&cool));
    printme(reinterpret_cast<cool_t *>(&cool));
    printme(bypass_cast<cool_t *>(&lame));
    printme(reinterpret_cast<cool_t *>(&lame));
    printme(bypass_cast<cool_t *>(lame_null));
    printme(reinterpret_cast<cool_t *>(lame_null));

    // below 2 fail in ASAN
    printme(bypass_cast<cool_t *>(&j));
    printme(reinterpret_cast<cool_t *>(&j));
    return 0;
}
