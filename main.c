#include <assert.h>
#include <stdbool.h>
#include <stdio.h>


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

#define DEFINE_OWNED_VAL(type, name, value) \
    type borrow_c_internal##name = value; \
    bool borrow_c_internal_moved##name = false; \
    typedef type borrow_c_type##name;

#define BAD_ACCESS_MARKER_NAME3(var, file_name, line) POSSIBLE_USE_AFTER_MOVE_OF_ ## var ## _IN_ ## file_name ## _AT_LINE_ ## line
#define BAD_ACCESS_MARKER_NAME2(var, file_name, line) BAD_ACCESS_MARKER_NAME3(var, file_name, line)
#define BAD_ACCESS_MARKER_NAME(var) BAD_ACCESS_MARKER_NAME2(var, SAFE_FILE_NAME, __LINE__)

#define DETECT_BAD_ACESSS(name) \
    { \
        void BAD_ACCESS_MARKER_NAME(name) (); \
        if (borrow_c_internal_moved##name) { \
            BAD_ACCESS_MARKER_NAME(name) (); \
        } \
    }

// Similar pattern to above? Maybe create one top level macro to avoid repetition?
#define FORGOT_TO_END_READ_ONLY_ACCESS3(var, file_name) did_you_forget_to_close_read_only_access_of_## var ## _IN_ ## file_name ## _AT_LINE_
#define FORGOT_TO_END_READ_ONLY_ACCESS2(var, file_name) FORGOT_TO_END_READ_ONLY_ACCESS3(var, file_name)
#define FORGOT_TO_END_READ_ONLY_ACCESS(var) FORGOT_TO_END_READ_ONLY_ACCESS2(var, SAFE_FILE_NAME)

// TODO: prevent move_from and move_to from being the same
#define MOVE_OWNED_VALUE(move_from, move_to) \
    DETECT_BAD_ACESSS(move_from) \
    DETECT_BAD_ACESSS(move_to) \
    borrow_c_internal_moved##move_from = true; \
    borrow_c_internal_moved##move_to = false; \
    borrow_c_internal##move_to = borrow_c_internal##move_to;

#define READ_ONLY_ACCESS_BEGIN(name) \
    { \
        const int FORGOT_TO_END_READ_ONLY_ACCESS(name)=__LINE__; \
        DETECT_BAD_ACESSS(name) \
        const bool borrow_c_internal_moved_before_access##name = borrow_c_internal_moved##name; \
        borrow_c_internal_moved##name = true; \
        const borrow_c_type##name name = borrow_c_internal##name; \
        assert(!borrow_c_internal_moved_before_access##name);
    
// TODO: CHECK that value is currently being accessed...
// Maybe define some weird variable and then cast it to void at the end
// if you get a warning for unused variables you know you made a mistake?
#define READ_ONLY_ACCESS_END(name) \
        borrow_c_internal_moved##name = borrow_c_internal_moved_before_access##name; \
        (void)FORGOT_TO_END_READ_ONLY_ACCESS(name); \
    }

int main() {
    DEFINE_OWNED_VAL(int, myVar, 6);

    READ_ONLY_ACCESS_BEGIN(myVar)
        printf("myVar=%i line=%i\n", myVar, __LINE__);
    READ_ONLY_ACCESS_END(myVar)

    DEFINE_OWNED_VAL(int, myVar2, 6);

    READ_ONLY_ACCESS_BEGIN(myVar)
        // Error because you can't move myVar whilst we have access to its value
        //MOVE_OWNED_VALUE(myVar, myVar2);
        printf("myVar=%i line=%i\n", myVar, __LINE__);
    READ_ONLY_ACCESS_END(myVar)

    MOVE_OWNED_VALUE(myVar, myVar2);

    READ_ONLY_ACCESS_BEGIN(myVar2)
        printf("myVar=%i line=%i\n", myVar2, __LINE__);
    READ_ONLY_ACCESS_END(myVar2)

    // Error because myVar is moved from
    // READ_ONLY_ACCESS_BEGIN(myVar)
    //     printf("myVar=%i line=%i x=%i\n", myVar, __LINE__);
    // READ_ONLY_ACCESS_END(myVar)

    int unowned = 7;
    DEFINE_OWNED_VAL(int*, ownedPtr, &unowned);

    READ_ONLY_ACCESS_BEGIN(ownedPtr)
        printf("pointed to by ownedPtr=%i\n", *ownedPtr);
    READ_ONLY_ACCESS_END(ownedPtr)


    return 0;
}
