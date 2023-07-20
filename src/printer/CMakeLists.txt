set(SRCS
    printer.c
)

add_library(printer STATIC ${SRCS})
target_include_directories(printer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(printer PRIVATE cpu)
target_link_libraries(printer PRIVATE queue)
target_link_libraries(printer PRIVATE utils)