#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME compilation_failure_dereference_write_null_c

#include "null_checked_ptr.h"

int main() {
    int foo = 2;
    NULL_CHECKED_PTR(int*, my_nc_ptr) = &foo;
    int my_nc_ptr_value = 0;
    
    ASSIGN_NC_PTR(my_nc_ptr, NULL);

    /* Errors because it would dereference a NULL pointer */
    DEREFERENCE_NC_PTR_WRITE(my_nc_ptr, my_nc_ptr_value);
    assert(my_nc_ptr_value == 2);
    printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);

    return 0;
}
