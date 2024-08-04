add_executable(firmware main.cpp)

target_link_libraries(firmware
    libopencm3
    common
    display
    i2c
    map
    systick
)

set(LINKER_SCRIPT_FILE ${CMAKE_SOURCE_DIR}/linker/STM32F411xE.ld)
stm32_add_linker_script(firmware PRIVATE ${LINKER_SCRIPT_FILE})
