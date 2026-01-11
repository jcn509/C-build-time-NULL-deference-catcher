#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME build_failure_addition_assignment_c

#include "null_checked_ptr.h"

int main(void) {
    NC_PTR(int*, my_nc_ptr) = NULL;

    /* Incrementing a NULL pointer will cause build failure */
    NC_PTR_ADDITION_ASSIGNMENT(my_nc_ptr, 2);

    return 0;
}
