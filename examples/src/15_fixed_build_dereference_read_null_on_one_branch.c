#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NC_PTR_SAFE_FILE_NAME fixed_build_dereference_read_null_on_one_branch_c

#include "null_checked_ptr.h"

int main(void) {
    int foo = 2;
    NC_PTR(int*, my_nc_ptr) = &foo;
    int my_nc_ptr_value = 0;

    srand((unsigned int)time(NULL));
    if(rand() & 1) {
        NC_PTR_ASSIGN(my_nc_ptr, NULL);
    }

    /*
        If we do a null check, then the compiler will know that the pointer is
        not NULL
    */
    if (!NC_PTR_IS_NULL(my_nc_ptr)) {
        NC_PTR_DEREFERENCE_READ(my_nc_ptr_value, my_nc_ptr);
        assert(my_nc_ptr_value == 2);
        printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);
    }
    else {
        printf("my_nc_ptr_value is NULL line=%i\n", __LINE__);
    }

    return 0;
}
