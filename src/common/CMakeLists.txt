add_library(common STATIC types.h syscalls.c)
target_include_directories(common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

set(DEBUG_BREAK_ENABLE 1)

# set(COMMON_COMPILE_DEFINITIONS
#     -D${PLATFORM_UPPER_CASE}
#     -D${CORTEX_ARCHITECTURE}
#     -DDEBUG_BREAK_ENABLE=${DEBUG_BREAK_ENABLE}
# )

# if(ENABLE_UNIT_TEST)
#     list(APPEND COMMON_COMPILE_DEFINITIONS -DUNIT_TEST)
# endif()

add_opt_define(
    ${PLATFORM_UPPER_CASE}
    ${CORTEX_ARCHITECTURE}
    DEBUG_BREAK_ENABLE=${DEBUG_BREAK_ENABLE}
)

if(ENABLE_UNIT_TEST)
    add_opt_define(UNIT_TEST)
endif()
    
target_compile_definitions(common PUBLIC ${COMMON_COMPILE_DEFINITIONS})
include(AddCompileLinkOptions)
add_compile_link_options(common PUBLIC)
