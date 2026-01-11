#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME build_failure_post_increment_c

#include "null_checked_ptr.h"

int main(void) {
    NC_PTR(int*, my_nc_ptr) = NULL;

    /* Incrementing a NULL pointer will cause build failure */
    NC_PTR_POST_INCREMENT(my_nc_ptr);

    /* Silence compiler warning about unused variable */
    if (!NC_PTR_IS_NULL(my_nc_ptr)) {
        int my_nc_ptr_value = 0;
        NC_PTR_DEREFERENCE_READ(my_nc_ptr_value, my_nc_ptr);
        (void)my_nc_ptr_value;
    }

    return 0;
}
