
function(add_example example_name sources)
    add_executable(${example_name} ${sources})
    target_link_libraries(${example_name} ${PROJECT_NAME})
    add_test(NAME ${example_name} COMMAND ${example_name})
endfunction()

add_example(basics src/1_basics.c)
add_example(functions src/2_functions.c)
add_example(arrays src/3_arrays.c)
add_example(structs src/4_structs.c)
