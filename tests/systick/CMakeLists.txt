# SUT
set(SUT_SRC_DIR ${SOURCE_DIR}/systick)
add_library(systick STATIC
    ${SUT_SRC_DIR}/systick.cpp
    ${SUT_SRC_DIR}/systick.hpp
)

target_include_directories(systick PUBLIC ${SUT_SRC_DIR})
target_link_libraries(systick common ut_systick_stub_libopencm3)

# Stubs
add_subdirectory(libopencm3)

# Another libraries

# UT configuration
add_executable(ut_systick "ut_systick.cpp")
target_link_libraries(ut_systick PRIVATE acutest systick)
add_unit_test(ut_systick)
