add_library(i2c STATIC
    i2c1.cpp
    i2c1.hpp
)

target_include_directories(i2c PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(i2c common libopencm3 systick)
