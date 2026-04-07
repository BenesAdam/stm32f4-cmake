add_library(header STATIC
    header_checker.cpp
    header_checker.hpp
    header_structure.hpp
    header_structure.cpp
    sha256.hpp
    sha256.cpp
    rsa_2048.hpp
    rsa_2048.cpp
)

target_include_directories(header PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(header common libopencm3 systick)
