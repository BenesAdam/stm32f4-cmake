add_subdirectory(common)

if(NOT ENABLE_UNIT_TEST)
    add_subdirectory(systick)
    add_subdirectory(display)
    add_subdirectory(i2c)
    add_subdirectory(map)
    add_subdirectory(noinit_vars)
    add_subdirectory(errorhandler)

    # Add here components that are not compatible with STM32F1
    if(NOT ENABLE_PLATFORM_STM32F1)
        
    endif()
    
endif()
