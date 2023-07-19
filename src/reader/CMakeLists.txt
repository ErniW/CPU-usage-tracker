set(SRCS
    reader.c
)

add_library(reader STATIC ${SRCS})
target_include_directories(reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(reader PRIVATE cpu)
target_link_libraries(reader PRIVATE queue)
target_link_libraries(reader PRIVATE utils)