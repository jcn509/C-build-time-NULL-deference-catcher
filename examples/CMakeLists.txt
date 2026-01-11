
function(add_example example_name sources)
    add_executable(${example_name} ${sources})
    target_link_libraries(${example_name} ${PROJECT_NAME})
endfunction()

add_example(basic src/basic.c)
add_example(function src/function.c)
add_example(array src/array.c)