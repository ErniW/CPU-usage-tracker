cmake_minimum_required(VERSION 3.12)
project(cpu_usage_tracker VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_COMPILER clang)

set(SRCS
    src/main.c
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_C_COMPILER MATCHES "clang")
    set(CFLAGS "-Weverything")
elseif(CMAKE_C_COMPILER MATCHES "gcc")
    set(CFLAGS "-Wall -Wextra")
endif()

add_subdirectory(src/cpu)
add_subdirectory(src/reader)
add_subdirectory(src/analyzer)
add_subdirectory(src/printer)
add_subdirectory(src/queue)
add_subdirectory(src/utils)
add_subdirectory(tests)

add_executable(cpu_usage_tracker_prod ${SRCS})
set_target_properties(cpu_usage_tracker_prod PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/prod"
)
target_compile_options(cpu_usage_tracker_prod PRIVATE ${CFLAGS})
target_link_libraries(cpu_usage_tracker_prod PRIVATE 
    cpu
    reader
    analyzer
    printer
    queue
    utils
)

add_custom_target(compile ALL DEPENDS cpu_usage_tracker_prod)

add_executable(cpu_usage_tracker_debug ${SRCS})
set_target_properties(cpu_usage_tracker_debug PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/debug"
)
target_compile_options(cpu_usage_tracker_debug PRIVATE ${CFLAGS} -DDEBUG)
target_link_libraries(cpu_usage_tracker_debug PRIVATE 
    cpu
    reader
    analyzer
    printer
    queue
    utils
    tests
)

add_custom_target(debug ALL DEPENDS cpu_usage_tracker_debug)