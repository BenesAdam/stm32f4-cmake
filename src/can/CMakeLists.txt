add_library(can STATIC
    can_cfg.cpp
    can_cfg.hpp
    can_message.cpp
    can_message.hpp    
    can1.cpp
    can1.hpp
)

target_include_directories(can PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(can common libopencm3)
