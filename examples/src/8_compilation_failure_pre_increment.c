#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME compilation_failure_pre_increment_c

#include "null_checked_ptr.h"

int main() {
    NULL_CHECKED_PTR(int*, my_nc_ptr) = NULL;

    /* Incrementing a NULL pointer will cause build failure */
    PRE_INCREMENT_NC_PTR(my_nc_ptr);

    return 0;
}
