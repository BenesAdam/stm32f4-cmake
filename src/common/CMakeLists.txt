add_library(common INTERFACE types.h)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if(ENABLE_UNIT_TEST)
    target_compile_definitions(common INTERFACE "-DUNIT_TEST")
endif()

include(AddCompileLinkOptions)
add_compile_link_options(common INTERFACE)
