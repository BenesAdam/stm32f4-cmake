add_executable(firmware main.cpp)

target_link_libraries(firmware
    libopencm3
    common
    display
    i2c
    map
    noinit_vars
    systick
    errorhandler
)

set(LINKER_SCRIPT_FILE ${PROJECT_SOURCE_DIR}/linker/STM32F411xE.ld)
set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/firmware.map)

target_link_options(firmware PRIVATE
    -T "${LINKER_SCRIPT_FILE}"
    -Wl,-Map=${MAP_FILE},--print-memory-usage
)
