set(SRCS
    tests.c
)

add_library(tests STATIC ${SRCS})
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests PRIVATE cpu)