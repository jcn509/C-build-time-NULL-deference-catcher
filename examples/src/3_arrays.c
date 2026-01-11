/**
 * This example showcases several different methods for iterating through
 * an array using a NULL-checked pointer forwards and backwards. Several
 * of the examples appear to be too complex for my compiler to determine
 * that the pointer is not NULL and so they show the limitations of this
 * approach. 
 */

#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME arrays_c

#include "null_checked_ptr.h"


static void IterateThroughArrayForwards() {
    int foo_array[] =  {6, 5, 3, 7, 8};
    NC_PTR(int*, my_nc_array_ptr) = foo_array;
    const size_t foo_array_size = sizeof(foo_array) / sizeof(foo_array[0]);
    size_t i=0;
    /*
        There are several methods for iterating through an array pointed
        to by a NULL-checked pointer which are listed below
    */


    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        NC_PTR_DEREFERENCE_READ_OFFSET(element_value, my_nc_array_ptr, i);
        printf("Read offset my_nc_array_ptr[%i]=%i\n", (int)i, element_value);
    }

    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        NC_PTR_DEREFERENCE_READ(element_value, my_nc_array_ptr);
        printf("pre increment my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        assert(!NC_PTR_IS_NULL(my_nc_array_ptr));
        /*
            This method appears to prevent my compiler from
            determining that the pointer is not NULL in debug
            mode (but not release mode) so I have to add a check
        */
        if(!NC_PTR_IS_NULL(my_nc_array_ptr)) {
            /* ++my_nc_array_ptr */
            NC_PTR_PRE_INCREMENT(my_nc_array_ptr);
        }
    }
    /* Reset the pointer value */
    NC_PTR_ASSIGN(my_nc_array_ptr, foo_array);

    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        NC_PTR_DEREFERENCE_READ(element_value, my_nc_array_ptr);
        assert(element_value == foo_array[i]);
        printf("post increment my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        /*
            This method appears to prevent my compiler from
            determining that the pointer is not NULL in debug
            mode (but not release mode) so I have to add a check
        */
        if(!NC_PTR_IS_NULL(my_nc_array_ptr)) {
            /* my_nc_array_ptr++ */
            NC_PTR_POST_INCREMENT(my_nc_array_ptr);
        }
    }
    NC_PTR_ASSIGN(my_nc_array_ptr, foo_array);

    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        NC_PTR_DEREFERENCE_READ(element_value, my_nc_array_ptr);
        assert(element_value == foo_array[i]);
        printf("Add to ptr my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        /*
            This method appears to prevent my compiler from
            determining that the pointer is not NULL in debug
            mode (but not release mode) so I have to add a check
        */
        if(!NC_PTR_IS_NULL(my_nc_array_ptr)) {
            /* my_nc_array_ptr += 1 */
            NC_PTR_ADDITION_ASSIGNMENT(my_nc_array_ptr, 1);
        }
    }
    NC_PTR_ASSIGN(my_nc_array_ptr, foo_array);
}

static void IterateThroughArrayBackwards() {
    const int foo_array[] =  {6, 5, 3, 7, 8};
    const size_t foo_array_size = 5;
    const int* const foo_array_last_element = foo_array + foo_array_size - 1;
    NC_PTR(const int*, my_nc_array_ptr) = foo_array_last_element;
    size_t i=0;

    /*
        There are several methods for iterating through an array pointed
        to by a NULL-checked pointer which are listed below
    */

    assert(*foo_array_last_element == 8);

    for(i=0; i < foo_array_size; i++) {
        int element_value = 0;
        NC_PTR_DEREFERENCE_READ_OFFSET(element_value, my_nc_array_ptr, -i);
        printf("Backwards read offset my_nc_array_ptr[%i]=%i *foo_array_last_element=%i *(foo_array_last_element -i)=%i\n", (int)i, element_value, *foo_array_last_element, *(foo_array_last_element -i));
        assert(element_value == foo_array[foo_array_size - (i +1)]);
    }

    i=foo_array_size-1;
    assert(*foo_array_last_element == foo_array[i]);
    while(1) {
        int element_value = 0;
        /*
            My compiler struggles to prove that the pointer is not NULL
            when iterating backwards through an array using this method
            in debug mode (but not in release mode) so I have added this
            check
        */
        assert(!NC_PTR_IS_NULL(my_nc_array_ptr));
        if(!NC_PTR_IS_NULL(my_nc_array_ptr)) {
            NC_PTR_DEREFERENCE_READ(element_value, my_nc_array_ptr);
            printf("Backwards pre decrement my_nc_array_ptr[%i]=%i\n", (int)i, element_value);
            assert(element_value == foo_array[i]);
       
            /* --my_nc_array_ptr */
            NC_PTR_PRE_DECREMENT(my_nc_array_ptr);
        }

        if(i == 0) {
            break;
        }
        i--;
    }

    /* Reset the pointer value */
    NC_PTR_ASSIGN(my_nc_array_ptr, foo_array_last_element);
    i=foo_array_size-1;
    while(1) {
        int element_value = 0;
        /*
            My compiler struggles to prove that the pointer is not NULL
            when iterating backwards through an array using this method
            in debug mode (but not in release mode) so I have added this
            check
        */
        assert(!NC_PTR_IS_NULL(my_nc_array_ptr));
        if(!NC_PTR_IS_NULL(my_nc_array_ptr)) {
            NC_PTR_DEREFERENCE_READ(element_value, my_nc_array_ptr);
            assert(element_value == foo_array[i]);
            printf("Backwards post decrement my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

            /* my_nc_array_ptr-- */
            NC_PTR_POST_DECREMENT(my_nc_array_ptr);
        }

        if(i == 0) {
            break;
        }
        i--;
    }

    NC_PTR_ASSIGN(my_nc_array_ptr, foo_array_last_element);
    i=foo_array_size-1;
    while(1) {
        int element_value = 0;
        /*
            My compiler struggles to prove that the pointer is not NULL
            when iterating backwards through an array using this method
            in debug mode (but not in release mode) so I have added this
            check
        */
        assert(!NC_PTR_IS_NULL(my_nc_array_ptr));
        if(!NC_PTR_IS_NULL(my_nc_array_ptr)) {
            NC_PTR_DEREFERENCE_READ(element_value, my_nc_array_ptr);
            assert(element_value == foo_array[i]);
            printf("Backwards Subtract from ptr my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

            /* my_nc_array_ptr -= 1 */
            NC_PTR_SUBTRACTION_ASSIGNMENT(my_nc_array_ptr, 1);
        }

        if(i == 0) {
            break;
        }
        i--;
    }
}

int main() {
    IterateThroughArrayForwards();
    IterateThroughArrayBackwards();

    return 0;
}
