#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME build_failure_pre_decrement_c

#include "null_checked_ptr.h"

int main(void) {
    NC_PTR(int*, my_nc_ptr) = NULL;

    /* Decrementing a NULL pointer will cause build failure */
    NC_PTR_PRE_DECREMENT(my_nc_ptr);

    return 0;
}
