#include <assert.h>
#include <stdio.h>

#define NC_PTR_SAFE_FILE_NAME structs_c

#include "null_checked_ptr.h"

typedef struct Sprite {
    int x;
    int y;
    int z;
    int width;
    int height;
    NULL_CHECKED_PTR(unsigned char*, nc_image_data);
} Sprite;

int main() {
    Sprite my_sprite = {
        1,
        2,
        3,
        4,
        5,
        NULL
    };
    Sprite* const my_sprite_ptr = &my_sprite;

    unsigned char original_image[] = {66, 77, 22};
    unsigned char new_image[] = {55, 12, 35};
    unsigned char new_image_2[] = {112, 98, 54};
    const size_t image_size = 3;

    size_t i = 0;

    NULL_CHECKED_PTR(unsigned char*, nc_image_data_copy) = NULL;

    /* There is a special macro used to assign to a NULL checked pointer field */
    STRUCT_FIELD_ASSIGN_NC_PTR(my_sprite, nc_image_data, original_image);

    /*
        In order to access the data in a NULL checked pointer field of the struct
        we first need to copy the fields value into a NULL checked pointer
    */
    STRUCT_FIELD_NC_PTR_COPY(nc_image_data_copy, my_sprite, nc_image_data);
    for(i=0; i<image_size; i++) {
        unsigned char image_element = 0;
        DEREFERENCE_NC_PTR_READ_OFFSET(image_element, nc_image_data_copy, i);
        assert(image_element == original_image[i]);
        printf("original image element %i = %i\n", (int)i, image_element);
    }

    STRUCT_FIELD_ASSIGN_NC_PTR(my_sprite, nc_image_data, new_image);
    /* Must re-copy the pointer value now that it has been updated */
    STRUCT_FIELD_NC_PTR_COPY(nc_image_data_copy, my_sprite, nc_image_data);
    for(i=0; i<image_size; i++) {
        unsigned char image_element = 0;
        DEREFERENCE_NC_PTR_READ_OFFSET(image_element, nc_image_data_copy, i);
        assert(image_element == new_image[i]);
        printf("updated image element %i = %i\n", (int)i, image_element);
    }

    /* We use the same macros if we have a pointer to the struct */
    STRUCT_FIELD_ASSIGN_NC_PTR(*my_sprite_ptr, nc_image_data, new_image_2);
    STRUCT_FIELD_NC_PTR_COPY(nc_image_data_copy, *my_sprite_ptr, nc_image_data);
    for(i=0; i<image_size; i++) {
        unsigned char image_element = 0;
        DEREFERENCE_NC_PTR_READ_OFFSET(image_element, nc_image_data_copy, i);
        assert(image_element == new_image_2[i]);
        printf("updated image 2 element %i = %i\n", (int)i, image_element);
    }

    /*
        If you have a NULL checked pointer to the struct then you can either
        dereference it or use UNSAFE_CONVERT_NC_PTR_TO_RAW_PTR and then proceed
        as above
    */

    return 0;
}
