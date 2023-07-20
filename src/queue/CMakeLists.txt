set(SRCS
    queue.c
)

add_library(queue STATIC ${SRCS})
target_include_directories(queue PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(queue PRIVATE cpu)