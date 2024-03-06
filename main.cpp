#include "functions/smart_pointers.hpp"
#include "functions/try_scandir.hpp"

int main() {
    int status = 0;
    // status += access_types();
    // status += bad_std_copy();
    // status += smart_pointers();
    status += try_scandir();

    return status;
}
