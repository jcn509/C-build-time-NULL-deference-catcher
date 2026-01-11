#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME basics_c

#include "null_checked_ptr.h"

int main() {
    int foo = 2;
    int foo2 = 53;
    NULL_CHECKED_PTR(int*, my_nc_ptr) = &foo;
    int my_nc_ptr_value = 0;

    /* 
        If we set my_nc_ptr to NULL here we would see
        Errors when trying to dereference the pointer below
    */
    /* ASSIGN_NC_PTR(my_nc_ptr, NULL); */

    /*
        It is impossible to access the underlying pointer directly.
        If you try to do so you will get a build error. You must
        use the macros provided by this library.
        i.e. we cannot do:
          - my_nc_ptr = NULL
          - *my_nc_ptr = 12
          - int x = *my_nc_ptr
    */

    DEREFERENCE_NC_PTR_READ(my_nc_ptr_value, my_nc_ptr);
    assert(my_nc_ptr_value == 2);
    printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);

    foo++;
    DEREFERENCE_NC_PTR_READ(my_nc_ptr_value, my_nc_ptr);
    assert(my_nc_ptr_value == 3);
    printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);

    DEREFERENCE_NC_PTR_WRITE(my_nc_ptr, 5);
    DEREFERENCE_NC_PTR_READ(my_nc_ptr_value, my_nc_ptr);
    assert(my_nc_ptr_value == 5);
    printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);

    ASSIGN_NC_PTR(my_nc_ptr, &foo2);
    DEREFERENCE_NC_PTR_READ(my_nc_ptr_value, my_nc_ptr);
    assert(my_nc_ptr_value == 53);
    printf("my_nc_ptr_value=%i line=%i\n", my_nc_ptr_value, __LINE__);

    return 0;
}
