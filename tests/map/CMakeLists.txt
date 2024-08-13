# SUT
set(SUT_SRC_DIR ${SOURCE_DIR}/map)
add_library(map INTERFACE
    ${SUT_SRC_DIR}/const_size_allocator.hpp
    ${SUT_SRC_DIR}/map.hpp
)

target_include_directories(map INTERFACE ${SUT_SRC_DIR})
target_link_libraries(map INTERFACE common)

# Stubs

# Another libraries

# UT configuration
add_executable(ut_map "ut_map.cpp")
target_link_libraries(ut_map PRIVATE acutest map)
add_unit_test(ut_map)
