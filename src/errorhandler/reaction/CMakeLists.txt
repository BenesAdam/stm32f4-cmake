add_library(error_reaction STATIC
    error_reaction.hpp
    reset_react.cpp
    reset_react.hpp
    none_react.hpp
    none_react.cpp
)

target_include_directories(error_reaction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(error_reaction common libopencm3)
