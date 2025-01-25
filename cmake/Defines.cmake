include_guard(GLOBAL)

set(COMMON_COMPILE_DEFINITIONS "" CACHE STRING "Common compile definitions")

macro(add_opt_define)
    get_property(current_definitions GLOBAL PROPERTY COMMON_COMPILE_DEFINITIONS)
    foreach(define ${ARGN})
        list(APPEND current_definitions ${define})
    endforeach()
    set_property(GLOBAL PROPERTY COMMON_COMPILE_DEFINITIONS "${current_definitions}")
endmacro()
    
macro(finalize_opt_define)
    get_property(current_definitions GLOBAL PROPERTY COMMON_COMPILE_DEFINITIONS)
    list(TRANSFORM current_definitions PREPEND -D)
    set_property(GLOBAL PROPERTY COMMON_COMPILE_DEFINITIONS "${current_definitions}")
    target_compile_definitions(common PUBLIC ${current_definitions})
endmacro()
