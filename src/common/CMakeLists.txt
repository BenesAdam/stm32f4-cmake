add_library(common INTERFACE types.h)
target_include_directories(common INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

set(COMMON_COMPILE_DEFINITIONS
    -D${PLATFORM_UPPER_CASE}
    -D${CORTEX_ARCHITECTURE}
)

if(ENABLE_UNIT_TEST)
    list(APPEND COMMON_COMPILE_DEFINITIONS -DUNIT_TEST)
endif()
    
target_compile_definitions(common INTERFACE ${COMMON_COMPILE_DEFINITIONS})
include(AddCompileLinkOptions)
add_compile_link_options(common INTERFACE)
