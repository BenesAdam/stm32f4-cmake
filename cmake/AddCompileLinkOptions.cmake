if(ENABLE_PLATFORM_STM32F1)
    set(PLATFORM_COMPILE_OPTIONS
        -mcpu=cortex-m3
        -mfloat-abi=soft
    )
else()
    set(PLATFORM_COMPILE_OPTIONS
        -mcpu=cortex-m4
        -mfloat-abi=hard
        -mfpu=fpv4-sp-d16
    )
endif()

function(add_compile_link_options target visibility)
    set(COMPILE_OPTIONS
        --static
        -nostartfiles
        -fno-common
        -mthumb
        --specs=nano.specs
        ${PLATFORM_COMPILE_OPTIONS}
    )

    set(LINKER_OPTIONS
        ${COMPILE_OPTIONS}
        -Wl,--cref
    )

    target_compile_options(${target} ${visibility} ${COMPILE_OPTIONS})
    target_link_options(${target} ${visibility} ${LINKER_OPTIONS})
endfunction()
