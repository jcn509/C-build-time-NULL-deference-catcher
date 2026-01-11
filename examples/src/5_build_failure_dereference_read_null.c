#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME build_failure_dereference_read_null_c

#include "null_checked_ptr.h"

int main() {
    int foo = 2;
    NC_PTR(int*, my_nc_ptr) = &foo;
    int my_nc_ptr_value = 0;
    
    NC_PTR_ASSIGN(my_nc_ptr, NULL);

    /* Errors because it would dereference a NULL pointer */
    NC_PTR_DEREFERENCE_READ(my_nc_ptr_value, my_nc_ptr);
    assert(my_nc_ptr_value == 2);
    printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);

    return 0;
}
