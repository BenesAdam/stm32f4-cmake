add_library(noinit_vars STATIC
    noinit_vars.cpp
    noinit_vars.hpp
)

target_include_directories(noinit_vars PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(noinit_vars common)
