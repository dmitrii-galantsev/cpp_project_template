/*
 * Copyright TEST
 * This file showcases different ways of accessing a variable
 */

#include "functions.hpp"

#include <cstdio>

/*
 * Declaration of a reference without instantiation is not allowed
 * The line below will not compile:
 * int & my_reference;
 */

void pass_by_value(int arg) {
    printf("arg: %p:%d\n", reinterpret_cast<void *>(&arg), arg);
    arg++;
    printf("arg: %p:%d\n", reinterpret_cast<void *>(&arg), arg);
}

void pass_by_reference(int &arg) {
    printf("arg: %p:%d\n", reinterpret_cast<void *>(&arg), arg);
    arg++;
    printf("arg: %p:%d\n", reinterpret_cast<void *>(&arg), arg);
}

void pass_by_pointer(int *arg) {
    printf("arg: %p:%d\n", reinterpret_cast<void *>(arg), *arg);
    (*arg)++;
    printf("arg: %p:%d\n", reinterpret_cast<void *>(arg), *arg);
}

void run_functions() {
    int my_int = 0;
    printf("initial\n");
    printf("my_int = %d\n\n", my_int);

    printf("pass_by_value\n");
    pass_by_value(my_int);
    printf("my_int = %d\n\n", my_int);

    printf("pass_by_reference\n");
    pass_by_reference(my_int);
    printf("my_int = %d\n\n", my_int);
}
