add_library(systick STATIC
    systick.cpp
    systick.hpp
)

target_include_directories(systick PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(systick common libopencm3)
