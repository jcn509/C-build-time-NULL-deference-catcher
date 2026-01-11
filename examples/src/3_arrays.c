/**
 * This example showcases several different methods for iterating through
 * an array using a NULL checked pointer forwards and backwards. Several
 * of the examples appear to be too complex for my compiler to determine
 * that the pointer is not NULL and so they show the limitations of this
 * approach. 
 */

#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME arrays_c

#include "null_checked_ptr.h"


static void IterateThroughArrayForwards() {
    int foo_array[] =  {6, 5, 3, 7, 8};
    NULL_CHECKED_PTR(int*, my_nc_array_ptr) = foo_array;
    const size_t foo_array_size = sizeof(foo_array) / sizeof(foo_array[0]);
    size_t i=0;
    /*
        There are several methods for iterating through an array pointed
        to by a NULL checked pointer which are listed below
    */


    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        DEREFERENCE_NC_PTR_READ_OFFSET(element_value, my_nc_array_ptr, i);
        printf("Read offset my_nc_array_ptr[%i]=%i\n", (int)i, element_value);
    }

    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        DEREFERENCE_NC_PTR_READ(element_value, my_nc_array_ptr);
        /*
            This method appears to prevent my compiler from
            determining that the pointer is not NULL in debug
            mode (but not release mode) so I have to add a check
        */
        if(!IS_NC_PTR_NULL(my_nc_array_ptr)) {
            /* ++my_nc_array_ptr */
            PRE_INCREMENT_NC_PTR(my_nc_array_ptr);
        }
        printf("pre increment my_nc_array_ptr[%i]=%i\n", (int)i, element_value);
    }
    /* Reset the pointer value */
    ASSIGN_NC_PTR(my_nc_array_ptr, foo_array);

    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        DEREFERENCE_NC_PTR_READ(element_value, my_nc_array_ptr);

        /* my_nc_array_ptr++ */
        POST_INCREMENT_NC_PTR(my_nc_array_ptr);

        printf("post increment my_nc_array_ptr[%i]=%i\n", (int)i, element_value);
    }
    ASSIGN_NC_PTR(my_nc_array_ptr, foo_array);

    for(i=0; i<foo_array_size; i++) {
        int element_value = 0;
        DEREFERENCE_NC_PTR_READ(element_value, my_nc_array_ptr);

        /* my_nc_array_ptr += 1 */
        INCREASE_NC_PTR(my_nc_array_ptr, 1);

        printf("Add to ptr my_nc_array_ptr[%i]=%i\n", (int)i, element_value);
    }
    ASSIGN_NC_PTR(my_nc_array_ptr, foo_array);
}

static void IterateThroughArrayBackwards() {
    int foo_array[] =  {6, 5, 3, 7, 8};
    const size_t foo_array_size = sizeof(foo_array) / sizeof(foo_array[0]);
    NULL_CHECKED_PTR(int*, my_nc_array_ptr) = foo_array + (foo_array_size - 1);
    size_t i=0;
    /*
        There are several methods for iterating through an array pointed
        to by a NULL checked pointer which are listed below
    */


    i=foo_array_size-1;
    while(1) {
        int element_value = 0;
        DEREFERENCE_NC_PTR_READ_OFFSET(element_value, my_nc_array_ptr, -i);
        printf("Read offset my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        if(i == 0) {
            break;
        }
        i--;
    }

    i=foo_array_size-1;
    while(1) {
        int element_value = 0;
        /*
            My compiler struggles to prove that the pointer is not NULL
            when iterating backwards through an array using this method
            in debug mode (but not in release mode) so I have added this
            check
        */
        if(!IS_NC_PTR_NULL(my_nc_array_ptr)) {
            DEREFERENCE_NC_PTR_READ(element_value, my_nc_array_ptr);
       
            /* --my_nc_array_ptr */
            PRE_DECREMENT_NC_PTR(my_nc_array_ptr);
        }
        
        printf("pre decrement my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        if(i == 0) {
            break;
        }
        i--;
    }
    /* Reset the pointer value */
    ASSIGN_NC_PTR(my_nc_array_ptr, foo_array + (foo_array_size - 1));

    i=foo_array_size-1;
    while(1) {
        int element_value = 0;
        /*
            My compiler struggles to prove that the pointer is not NULL
            when iterating backwards through an array using this method
            in debug mode (but not in release mode) so I have added this
            check
        */
        if(!IS_NC_PTR_NULL(my_nc_array_ptr)) {
            DEREFERENCE_NC_PTR_READ(element_value, my_nc_array_ptr);

            /* my_nc_array_ptr-- */
            POST_DECREMENT_NC_PTR(my_nc_array_ptr);
        }

        printf("post decrement my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        if(i == 0) {
            break;
        }
        i--;
    }
    ASSIGN_NC_PTR(my_nc_array_ptr, foo_array + (foo_array_size - 1));

    i=foo_array_size-1;
    while(1) {
        int element_value = 0;
        /*
            My compiler struggles to prove that the pointer is not NULL
            when iterating backwards through an array using this method
            in debug mode (but not in release mode) so I have added this
            check
        */
        if(!IS_NC_PTR_NULL(my_nc_array_ptr)) {
            DEREFERENCE_NC_PTR_READ(element_value, my_nc_array_ptr);

            /* my_nc_array_ptr -= 1 */
            DECREASE_NC_PTR(my_nc_array_ptr, 1);
        }

        printf("Subtract from ptr my_nc_array_ptr[%i]=%i\n", (int)i, element_value);

        if(i == 0) {
            break;
        }
        i--;
    }
    ASSIGN_NC_PTR(my_nc_array_ptr, foo_array + (foo_array_size - 1));
}

int main() {
    IterateThroughArrayForwards();
    IterateThroughArrayBackwards();

    return 0;
}
