add_subdirectory(common)

if(NOT ENABLE_UNIT_TEST)
    add_subdirectory(display)
    add_subdirectory(i2c)
    add_subdirectory(map)
    add_subdirectory(noinit_vars)
    add_subdirectory(systick)
endif()
