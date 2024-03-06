#include "try_scandir.hpp"

#include <dirent.h>

#include <cstdio>
#include <cstdlib>
#include <string>

bool containsString(std::string originalString, std::string substring) {
    return (originalString.find(substring) != std::string::npos);
}

int try_scandir() {
    struct dirent **namelist = nullptr;
    int n = 0;

    auto p = [](const struct dirent *d) -> int { return containsString(d->d_name, "rocm"); };
    n = scandir("/tmp/", &namelist, p, alphasort);
    if (n == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    while (n--) {
        printf("d_name: %s\n", namelist[n]->d_name);
        free(namelist[n]);
    }
    free(namelist);

    return EXIT_SUCCESS;
}
