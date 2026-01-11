#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME build_failure_convert_to_raw_ptr_null_c

#include "null_checked_ptr.h"

int main(void) {
    int foo = 2;
    NC_PTR(int*, my_nc_ptr) = NULL;
    int* my_raw_ptr = &foo;

    /* Fails because the my_nc_ptr is NULL */
    UNSAFE_NC_PTR_CONVERT_TO_RAW_PTR(my_raw_ptr, my_nc_ptr);

    (void)my_raw_ptr;

    return 0;
}
