#include "functions/access_types.hpp"
#include "functions/bad_std_copy.hpp"
#include "functions/smart_pointers.hpp"
#include "functions/timing.hpp"
#include "functions/try_amdsmi.hpp"
#include "functions/try_scandir.hpp"

int main() {
    int status = 0;
    // status += access_types();
    // status += bad_std_copy();
    // status += smart_pointers();
    // status += try_amdsmi();
    // status += try_scandir();
    status += timing();

    return status;
}
