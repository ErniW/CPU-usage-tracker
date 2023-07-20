cmake_minimum_required(VERSION 3.12)
project(cpu_usage_tracker VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_COMPILER gcc)

set(SRCS
    src/main.c
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_C_COMPILER MATCHES "clang")
    set(CMAKE_C_FLAGS "-Weverything")
elseif(CMAKE_C_COMPILER MATCHES "gcc")
    set(CMAKE_C_FLAGS "-Wall -Wextra")
endif()

add_subdirectory(src/cpu)
add_subdirectory(src/reader)
add_subdirectory(src/analyzer)
add_subdirectory(src/printer)
add_subdirectory(src/queue)
add_subdirectory(src/utils)
add_subdirectory(tests)

function(add_executable_target target_name)
    add_executable(${target_name} ${SRCS} ${ARGN})
    target_compile_options(${target_name} PRIVATE ${CFLAGS} $<$<CONFIG:Debug>:-DDEBUG>)
    set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
    target_link_libraries(${target_name} PRIVATE cpu reader analyzer printer queue utils $<$<CONFIG:Debug>:tests>)
endfunction()

add_executable_target(cpu_usage_tracker_release)
add_executable_target(cpu_usage_tracker_debug tests/tests.c)

add_custom_target(debug ALL DEPENDS cpu_usage_tracker_debug)
add_custom_target(release ALL DEPENDS cpu_usage_tracker_release)