add_library(display STATIC
    display_character_map.cpp
    display_character_map.hpp
    display.cpp
    display.hpp
)

target_include_directories(display PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(display common map i2c systick)
