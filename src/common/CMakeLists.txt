add_library(common STATIC types.h syscalls.c)
target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set(COMMON_COMPILE_DEFINITIONS
    -D${PLATFORM_UPPER_CASE}
    -D${CORTEX_ARCHITECTURE}
)

if(ENABLE_UNIT_TEST)
    list(APPEND COMMON_COMPILE_DEFINITIONS -DUNIT_TEST)
endif()
    
target_compile_definitions(common PUBLIC ${COMMON_COMPILE_DEFINITIONS})
include(AddCompileLinkOptions)
add_compile_link_options(common PUBLIC)
