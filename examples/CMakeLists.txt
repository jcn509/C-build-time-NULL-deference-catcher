
function(add_example example_name source)
    add_executable(${example_name} ${source})
    target_link_libraries(${example_name} ${PROJECT_NAME})
    add_test(NAME ${example_name} COMMAND ${example_name})
endfunction()


function(add_failure_example example_name source expected_error_message)
    # As we expect this build to fail we exclude it from the default
    # build (when we build without a target) and add a ctest test
    # to build it which should fail with the given error message
    add_executable(${example_name} ${source})
    target_link_libraries(${example_name} ${PROJECT_NAME})
    set_target_properties(${example_name} PROPERTIES
        EXCLUDE_FROM_ALL TRUE
        EXCLUDE_FROM_DEFAULT_BUILD TRUE
    )
    add_test(
        NAME ${example_name}
        COMMAND ${CMAKE_COMMAND} --build . --target ${example_name} --config $<CONFIG>
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set_tests_properties(${example_name} PROPERTIES
        PASS_REGULAR_EXPRESSION "${expected_error_message}"
    )

    # These should compile if we turn off NULL checks
    add_executable(${example_name}_null_check_disabled ${source})
    target_link_libraries(${example_name}_null_check_disabled ${PROJECT_NAME})
    target_compile_definitions(${example_name}_null_check_disabled PRIVATE NC_PTR_DISABLE_NULL_CHECKS=1)
endfunction()

add_example(basics src/1_basics.c)
add_example(functions src/2_functions.c)
add_example(arrays src/3_arrays.c)
add_example(structs src/4_structs.c)

add_failure_example(
    compilation_failure_dereference_read_null
    src/5_compilation_failure_dereference_read_null.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_dereference_read_null_c_on_line_16"
)

add_failure_example(
    compilation_failure_dereference_write_null
    src/6_compilation_failure_dereference_write_null.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_dereference_write_null_c_on_line_16"
)

add_failure_example(
    compilation_failure_convert_to_raw_ptr_null
    src/7_compilation_failure_convert_to_raw_ptr_null.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_convert_to_raw_ptr_null_c_on_line_14"
)

add_failure_example(
    compilation_failure_pre_increment
    src/8_compilation_failure_pre_increment.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_pre_increment_c_on_line_12"
)

add_failure_example(
    compilation_failure_pre_decrement
    src/9_compilation_failure_pre_decrement.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_pre_decrement_c_on_line_12"
)

add_failure_example(
    compilation_failure_post_increment
    src/10_compilation_failure_post_increment.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_post_increment_c_on_line_12"
)

add_failure_example(
    compilation_failure_post_decrement
    src/11_compilation_failure_post_decrement.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_post_decrement_c_on_line_12"
)

add_failure_example(
    compilation_failure_addition_assignment
    src/12_compilation_failure_addition_assignment.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_addition_assignment_c_on_line_12"
)

add_failure_example(
    compilation_failure_subtraction_assignment
    src/13_compilation_failure_subtraction_assignment.c
    "null_dereference_of_my_nc_ptr_possible_in_file_compilation_failure_subtraction_assignment_c_on_line_12"
)
