add_library(common INTERFACE types.h)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT ENABLE_UNIT_TEST)
    target_compile_definitions(common INTERFACE "-DUSE_LIBOPENCM3")
endif()
