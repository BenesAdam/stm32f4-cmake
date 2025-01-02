add_library(can STATIC
    can1.cpp
    can1.hpp
)

target_include_directories(can PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(can common libopencm3)
