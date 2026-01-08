#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


// Can I rely on compier optimisations removing dead code?
// i.e. have a call to a function that won't link because it doesn't exist
// if that function call is emmitted then we get a linker error?

// Try to do something similar for forcing null checks if the compiler
// cannot prove that it is not null (i.e. if the compiler cannot elimate
// the dead code).
// There might be weird cases where if we call a function in a different translation
// unit we get different results if LTO is enabled because it might allow
// it to prove that something is not null?
// Could mitigate this providing some kind of non null type?

// TODO: error if this is not found
// Seems like the build system could be modified to auto-define this for every file?
#define SAFE_FILE_NAME main_c

typedef struct TestType {
    int value;

} TestType;


// Maybe these will be useful to constrain types used? Not sure if the reference will cause issues?
// Maybe also use this https://stackoverflow.com/questions/53840498/how-to-assert-two-types-are-equal-in-c to check if void*?
#define CHECK_DEREFERENCE(type) \
    do { \
        type x; \
        (void)(*x); /* Attempting to dereference x */ \
    } while (0)
#define TYPE_HAS_POINTER_SIZE(T) _Static_assert(sizeof(T) == sizeof(void*), "Type is not a pointer")


// Maybe have one version that auto mallocs and sets the value of the new memory
// and one that 
#define DEFINE_UNIQUE_PTR(type, name, value) \
    type borrow_c_internal##name = value; \
    typedef type borrow_c_type##name;

#define BAD_ACCESS_MARKER_NAME3(var, file_name, line) POSSIBLE_USE_AFTER_MOVE_OF_ ## var ## _IN_ ## file_name ## _AT_LINE_ ## line
#define BAD_ACCESS_MARKER_NAME2(var, file_name, line) BAD_ACCESS_MARKER_NAME3(var, file_name, line)
#define BAD_ACCESS_MARKER_NAME(var) BAD_ACCESS_MARKER_NAME2(var, SAFE_FILE_NAME, __LINE__)

#define DETECT_BAD_ACESSS(name) \
    { \
        void BAD_ACCESS_MARKER_NAME(name) (); \
        if (borrow_c_internal##name == NULL) { \
            BAD_ACCESS_MARKER_NAME(name) (); \
        } \
    }

// Similar pattern to above? Maybe create one top level macro to avoid repetition?
// TODO: use an array of a size __LINE__ to embed a line number
#define FORGOT_TO_END_READ_ONLY_ACCESS3(var, file_name) did_you_forget_to_close_read_only_access_of_## var ## _IN_ ## file_name
#define FORGOT_TO_END_READ_ONLY_ACCESS2(var, file_name) FORGOT_TO_END_READ_ONLY_ACCESS3(var, file_name)
#define FORGOT_TO_END_READ_ONLY_ACCESS(var) FORGOT_TO_END_READ_ONLY_ACCESS2(var, SAFE_FILE_NAME)

// TODO: prevent move_from and move_to from being the same
// TODO: ensure that move_to is freed first
#define MOVE_OWNED_VALUE(move_from, move_to) \
    DETECT_BAD_ACESSS(move_from) \
    DETECT_BAD_ACESSS(move_to) \
    borrow_c_internal##move_to = borrow_c_internal##move_from; \
    borrow_c_internal##move_from = NULL;

// TODO: ensure old value freed first
#define SET(name, value) \
    borrow_c_internal##name = value;

#define IS_NULL(name) \
    (borrow_c_internal##name == nullptr)

#define SAFE_DEREFERENCE_READ(name, destination) \
    DETECT_BAD_ACESSS(name) \
    destination = *borrow_c_internal##name;

#define SAFE_DEREFERENCE_WRITE(name, value) \
    { \
        DETECT_BAD_ACESSS(name) \
        *borrow_c_internal##name = value; \
    }

// Have the cast so that you can't do e.g. UNSAFE_GET_RAW_POINTER(name)++ and
// change the value of the smart pointer
#define UNSAFE_GET_RAW_POINTER(name) \
   ((borrow_c_type##name)borrow_c_internal##name)

// TODO: try and prevent nested READ_ONLY_ACCESS_BEGIN for the same variable
#define ACCESS_BEGIN(name) \
    { \
        const int FORGOT_TO_END_READ_ONLY_ACCESS(name)[__LINE__]; \
        DETECT_BAD_ACESSS(name) \
        borrow_c_type##name const name = borrow_c_internal##name;
    
#define ACCESS_END(name) \
        (void)FORGOT_TO_END_READ_ONLY_ACCESS(name); \
    }

int main() {
    DEFINE_UNIQUE_PTR(int*, myVar, malloc(sizeof(int)));

    ACCESS_BEGIN(myVar)
        *myVar = 12;
        printf("myVar=%i line=%i\n", *myVar, __LINE__);
        (*myVar)++;
    ACCESS_END(myVar)

    ACCESS_BEGIN(myVar)
        printf("myVar=%i line=%i\n", *myVar, __LINE__);
    ACCESS_END(myVar)

    int counter = 2;
    SET(myVar, &counter);
    ACCESS_BEGIN(myVar)
        printf("myVar=%i line=%i\n", *myVar, __LINE__);
        counter++;
        printf("myVar=%i line=%i\n", *myVar, __LINE__);
    ACCESS_END(myVar)

    //SET(myVar, NULL);
    int* raw_myVar = UNSAFE_GET_RAW_POINTER(myVar);
    *raw_myVar = 500;
    raw_myVar++;
    ACCESS_BEGIN(myVar)
        printf("myVar=%i line=%i\n", *myVar, __LINE__);
    ACCESS_END(myVar)

    //SET(myVar, NULL);
    SAFE_DEREFERENCE_READ(myVar, const int myVarValue);
    printf("myVarValue=%i line=%i\n", myVarValue, __LINE__);
    SAFE_DEREFERENCE_WRITE(myVar, 5);
    SAFE_DEREFERENCE_READ(myVar, const int myVarValue2);
    printf("myVarValue2=%i line=%i\n", myVarValue2, __LINE__);

    return 0;
}
