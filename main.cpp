#include "functions/access_types.hpp"
#include "functions/bad_std_copy.hpp"
#include "functions/functions.hpp"
#include "functions/my_map.hpp"
#include "functions/smart_pointers.hpp"
#include "functions/timing.hpp"
#include "functions/try_scandir.hpp"

int main() {
    int status = 0;
    // status += functions();
    // status += access_types();
    // status += bad_std_copy();
    // status += smart_pointers();
    // status += try_scandir();
    // status += timing();
    status += my_map();

    return status;
}
