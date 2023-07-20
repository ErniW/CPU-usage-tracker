set(SRCS
    analyzer.c
)

add_library(analyzer STATIC ${SRCS})
target_include_directories(analyzer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(analyzer PRIVATE cpu)
target_link_libraries(analyzer PRIVATE utils)
target_link_libraries(analyzer PRIVATE queue)