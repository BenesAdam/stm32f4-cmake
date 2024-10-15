# SUT
set(SUT_SRC_DIR ${SOURCE_DIR}/bitset)
add_library(bitset INTERFACE
    ${SUT_SRC_DIR}/bitset.hpp
)

target_include_directories(bitset INTERFACE ${SUT_SRC_DIR})
target_link_libraries(bitset INTERFACE common)

# Stubs

# Another libraries

# UT configuration
add_executable(ut_bitset "ut_bitset.cpp")
target_link_libraries(ut_bitset PRIVATE acutest bitset)
add_unit_test(ut_bitset)
