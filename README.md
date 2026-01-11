# C build time NULL deference catcher

This library abuses your C compilers dead code elimination optimisations to catch potential NULL dereferences at build time with no runtime overhead.

## How does it work?

Consider the following code snippet (NC_PTR is short for NULL-checked pointer):

```c
#define NC_PTR_SAFE_FILE_NAME build_failure_dereference_read_null_c

int foo = 2;
NC_PTR(int*, my_nc_ptr) = &foo;
int my_nc_ptr_value = 0;

srand(time(NULL));
if(rand() & 1) {
    NC_PTR_ASSIGN(my_nc_ptr, NULL);
}

/* Errors because we MAY dereference a NULL pointer */
NC_PTR_DEREFERENCE_READ(my_nc_ptr_value, my_nc_ptr);
...
```

This expands out to:

```c
#define NC_PTR_SAFE_FILE_NAME build_failure_dereference_read_null_c

int foo = 2;
int* _PRIVATE_NC_PTR_DO_NOT_TOUCH_my_nc_ptr = &foo;
int my_nc_ptr_value = 0;

srand(time(NULL));
if(rand() & 1) {
    NC_PTR_ASSIGN(my_nc_ptr, NULL);
}

_PRIVATE_NC_PTR_DO_NOT_TOUCH_my_nc_ptr = NULL;
{
    {
        void null_dereference_of_my_nc_ptr_possible_in_file_build_failure_dereference_read_null_c_on_line_13 ();
        if (_PRIVATE_NC_PTR_DO_NOT_TOUCH_my_nc_ptr == NULL) {
            null_dereference_of_my_nc_ptr_possible_in_file_build_failure_dereference_read_null_c_on_line_13 ();
        } 
    };
    my_nc_ptr_value = *(_PRIVATE_NC_PTR_DO_NOT_TOUCH_my_nc_ptr + 0);
};
...
```

As you can see the Macros provided insert forward declarations to functions that do not exist and then call them.
If the compiler can prove that the pointer is not NULL (i.e. that if statement is dead code), then it will remove it.
If it cannot, then you will get a linker error containing the variable name, file name and line number where there
may be a NULL deference.
(null_dereference_of_my_nc_ptr_possible_in_file_build_failure_dereference_read_null_c_on_line_13 in the example above.)

The API also prevents you from accessing the pointer without using the macros provided so you can't accidentally
skip the NULL checks. Because the compiler must be eliminating the checks in order for it to successfully compile the file
you can be sure that there is no runtime overhead!

We can fix the example above by adding a (runtime) NULL check:

```c
#define NC_PTR_SAFE_FILE_NAME build_fixed_dereference_read_null_c

int foo = 2;
NC_PTR(int*, my_nc_ptr) = &foo;
int my_nc_ptr_value = 0;

srand(time(NULL));
if(rand() & 1) {
    NC_PTR_ASSIGN(my_nc_ptr, NULL);
}

if (!NC_PTR_IS_NULL(my_nc_ptr)) {
    NC_PTR_DEREFERENCE_READ(my_nc_ptr_value, my_nc_ptr);
}
...
```

We do not need to manually add NULL checks in cases where the compiler can see that the pointer cannot be NULL:

```c
#define NC_PTR_SAFE_FILE_NAME no_build_problems

int foo = 2;
NC_PTR(int*, my_nc_ptr) = &foo;
int my_nc_ptr_value = 0;

NC_PTR_DEREFERENCE_READ(my_nc_ptr_value, my_nc_ptr);
...
```

## How do I use it?

The library is header-only and shipped in a single header file in the single_include directory you just need to
add this to your include path. If you use CMake, then you could also add this project as a subdirectory and
add then link against c_null_checked_ptrs.

As this library depends on dead code elimination you **MUST** use an optimising compiler and you must
have dead code optimisation optimisations enabled. With Clang and GCC if you are
doing a debug build make sure you have -Og enabled. (Note: CMake will not add any optimisation
flags by default if you don't specify CMAKE_BUILD_TYPE. See CMakeLists.txt for a workaround.)

The API is very small and simple.
Look in the examples folder for a full walkthrough including examples of compilation failures
where potential NULL dereferences are detected. (The files are numbered and you should go through
them in order.)

If you find that compiler optimisations are causing issues when you try to use a debugger and
your compiler doesn't let you turn off problematic optimisations then you can set
NC_PTR_DISABLE_NULL_CHECKS=1 when compiling which will remove the compile time checks and replace
them with runtime asserts.

## Development

In order to automatically test the library build it in debug mode and then run ctest.
This will execute all of the examples that compile which include a number of assert
statements to make sure everything is working as expected. It will also build all of
the examples that do not build due to NULL references and make sure that the correct
linker error is output.
