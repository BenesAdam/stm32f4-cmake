add_executable(firmware main.cpp)
target_link_libraries(firmware
    bitset
    libopencm3
    common
    display
    i2c
    map
    noinit_vars
    systick
    errorhandler
)

set(LINKER_SCRIPT_FILE ${PROJECT_SOURCE_DIR}/linker/${PLATFORM_FULL_NAME_UPPER_CASE}.ld)
set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/firmware.map)

# Add link options to be able to append to this list
include(AddCompileLinkOptions)
add_compile_link_options(firmware PRIVATE)

# Append another linker options
target_link_options(firmware PRIVATE
    -T "${LINKER_SCRIPT_FILE}"
    -Wl,-Map=${MAP_FILE},--print-memory-usage

    # TODO: Find a better solution
    # Problem: definitions from syscalls.c are not taken into account and warning is generated
    #          warning: _getpid is not implemented and will always fail
    -Wl,--undefined=_getpid
    -Wl,--undefined=_kill
    -Wl,--undefined=_exit
    -Wl,--undefined=_write
    -Wl,--undefined=_read
    -Wl,--undefined=_fstat
    -Wl,--undefined=_isatty
    -Wl,--undefined=_lseek
    -Wl,--undefined=_sbrk
)
