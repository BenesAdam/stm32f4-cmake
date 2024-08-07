add_library(map INTERFACE
    const_size_allocator.hpp
    map.hpp
)

target_include_directories(map INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(map INTERFACE common)
