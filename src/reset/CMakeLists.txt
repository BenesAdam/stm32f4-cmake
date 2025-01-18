add_library(reset STATIC
    noinit_vars.cpp
    noinit_vars.hpp
    reset.cpp
    reset.hpp
    reset_inst.cpp
    reset_inst.hpp
)

target_include_directories(reset PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(reset common libopencm3)
