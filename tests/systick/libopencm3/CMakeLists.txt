set(LIBOPENCM3_SOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/libopencm3/cm3/nvic.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libopencm3/cm3/nvic.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/libopencm3/cm3/systick.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libopencm3/cm3/systick.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/libopencm3/stm32/rcc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/libopencm3/stm32/rcc.h"
)

add_library(ut_systick_stub_libopencm3 STATIC ${LIBOPENCM3_SOURCE_FILES})
target_include_directories(ut_systick_stub_libopencm3 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ut_systick_stub_libopencm3 common)
