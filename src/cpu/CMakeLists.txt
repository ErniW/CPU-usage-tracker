set(SRCS
    cpu.c
)

add_library(cpu STATIC ${SRCS})
target_include_directories(cpu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})