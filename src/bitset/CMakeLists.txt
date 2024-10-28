add_library(bitset INTERFACE
    bitset.hpp
)

target_include_directories(bitset INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(bitset INTERFACE common)
