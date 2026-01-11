cmake_minimum_required(VERSION 3.5)

project(
    c_null_checked_ptrs
    VERSION 0.0.1
    LANGUAGES C
)

# I want to support the oldest version
# of C that I can. You can use this
# library with newer versions if you
# want.
set (CMAKE_C_STANDARD 90)


# If no optimisations are enabled
# the NULL dereference checks will not work
# as the dead code cannot be eliminated
set(OPTIMISATION_FLAG_PREFIX "-O")
set(LOWEST_LEVEL_OPTIMISATION_FLAG "-Og")
if(MSVC)
    set(OPTIMISATION_FLAG_PREFIX "/O")
    set(LOWEST_LEVEL_OPTIMISATION_FLAG "/O1")
endif()
string(FIND "${CMAKE_C_FLAGS}" "${OPTIMISATION_FLAG_PREFIX}" existing_optimization)
if(existing_optimization EQUAL -1)
    # Check build-type specific flags if CMAKE_BUILD_TYPE is set
    if(CMAKE_BUILD_TYPE)
        string(TOUPPER "${CMAKE_BUILD_TYPE}" build_type_upper)
        set(config_flags_var "CMAKE_C_FLAGS_${build_type_upper}")
        
        if(DEFINED ${config_flags_var})
            string(FIND "${${config_flags_var}}" ${OPTIMISATION_FLAG_PREFIX} existing_optimization)
        endif()
    endif()
endif()
if(existing_optimization EQUAL -1)
    message(STATUS "Adding ${LOWEST_LEVEL_OPTIMISATION_FLAG} to CMAKE_C_FLAGS (no other optimisation {OPTIMISATION_FLAG_PREFIX} flag found in ${CMAKE_C_FLAGS})")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${LOWEST_LEVEL_OPTIMISATION_FLAG}")
endif()


if(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX /Za")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror")

    option(USE_SANITIZERS "Enable sanitizers (Address, Undefined Behavior)" OFF)
    option(SANITIZER_ADDRESS "Enable AddressSanitizer" OFF)
    option(SANITIZER_UNDEFINED "Enable UndefinedBehaviorSanitizer" OFF)

    if(USE_SANITIZERS)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer")
        set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fno-omit-frame-pointer")
        
        if(SANITIZER_ADDRESS)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address")
        endif()
        
        if(SANITIZER_UNDEFINED)
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined")
            set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=integer")
            set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=nullability")
        endif()
endif()

endif()

add_library(${PROJECT_NAME} INTERFACE)

include(GNUInstallDirs)
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/single_include>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config)
install(
    EXPORT ${PROJECT_NAME}-config
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/${PROJECT_NAME}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR NULL_CHECKED_PTRS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
