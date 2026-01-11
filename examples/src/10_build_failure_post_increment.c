#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME build_failure_post_increment_c

#include "null_checked_ptr.h"

int main() {
    NC_PTR(int*, my_nc_ptr) = NULL;

    /* Incrementing a NULL pointer will cause build failure */
    NC_PTR_POST_INCREMENT(my_nc_ptr);

    return 0;
}
