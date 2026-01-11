#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME compilation_failure_subtraction_assignment_c

#include "null_checked_ptr.h"

int main() {
    NC_PTR(int*, my_nc_ptr) = NULL;

    /* Decrementing a NULL pointer will cause build failure */
    NC_PTR_SUBTRACTION_ASSIGNMENT(my_nc_ptr, 2);

    return 0;
}
