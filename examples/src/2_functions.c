#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME functions_c

#include "null_checked_ptr.h"

/**
 * Prints the value of the pointer to variable to the screen
 */
void OutputVariable(NULL_CHECKED_PTR(const int* const, some_nc_ptr)) {
    /*
        We have to add an explicit NULL check
        as the compiler will not always be able to prove that
        someVariable is not NULL and so we would get an error
        below.
        Note that if the function were inlined then you might
        not need to add the NULL check because in the compiler
        may be able to prove that someVariable is not NULL.
    */
    if (IS_NC_PTR_NULL(some_nc_ptr)) {
        printf("OutputVariable someVariable is NULL!\n");
    }
    else {
        int some_nc_ptr_value = 0;
        DEREFERENCE_NC_PTR_READ(some_nc_ptr_value, some_nc_ptr);
        printf("OutputVariable *some_nc_ptr=%i\n", some_nc_ptr_value);
    }
}

/**
 * Prints the value of the pointer to variable to the screen
 * 
 * This version of the OutputVariable function is not safe
 * as we do not check if someVariable is NULL and as we use
 * a raw pointer and not a NULL_CHECKED_PTR We won't get a
 * build error...
 */
void OutputVariable_UNSAFE(const int* const some_nc_ptr) {
    /*
        In the unsafe version we don't need to add a NULL check.
        However, there there will be no build time error if a
        programmer passes a NULL to this function.
    */
    printf("OutputVariable_UNSAFE *some_nc_ptr=%i\n", *some_nc_ptr);
}

int main() {
    int foo = 2;
    NULL_CHECKED_PTR(int*, my_nc_ptr) = &foo;
    int* my_raw_ptr = NULL;

    /*
        We need to convert the NULL_CHECKED_PTR to a regular
        "raw" pointer in order to pass it to this function.
        We say this operation is "unsafe" to draw attention
        to it during code as we should only do this conversion
        when required if we want to catch NULL dereferences at
        compile time. Before the value is converted there is a
        compile-time check to make sure it is not NULL.

        In this case the function uses a NULL-checked
        pointer so it will be forced to check if the ptr is NULL
        before it can be used.
    */
    UNSAFE_CONVERT_NC_PTR_TO_RAW_PTR(my_raw_ptr, my_nc_ptr);
    assert(*my_raw_ptr == 2);
    OutputVariable(my_raw_ptr);

    /*
        Now the raw pointer and the NULL-checked pointer
        both point to the same integer
    */
    DEREFERENCE_NC_PTR_WRITE(my_nc_ptr, 5);
    assert(*my_raw_ptr == 5);
    /* 
        We can also pass the raw pointer to a function that does
        not use a NULL-checked pointer
    */
    OutputVariable_UNSAFE(my_raw_ptr);
    /* doing my_raw_ptr = NULL; would not effect my_nc_ptr */

    /*
        There is an even less safe alternative that does not
        perform a NULL check. You should only use this method
        if you want to access the pointer value to pass it to
        some code that does not use NULL-checked pointers and
        you do not care if it is NULL because that code can
        handle it.
        In this case OutputVariable_UNSAFE cannot correctly
        handle a NULL pointer so this is very risky.0
    */
    OutputVariable_UNSAFE(UNSAFE_CONVERT_NC_PTR_TO_RAW_PTR_NO_NULL_CHECK(my_nc_ptr));
    assert(*UNSAFE_CONVERT_NC_PTR_TO_RAW_PTR_NO_NULL_CHECK(my_nc_ptr) == 5);

    return 0;
}
