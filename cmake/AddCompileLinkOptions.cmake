function(add_compile_link_options target visibility)
    set(COMPILE_OPTIONS
        --static
        -nostartfiles
        -fno-common
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -mabi=aapcs
        --specs=nano.specs
    )

    set(LINKER_OPTIONS
        ${COMPILE_OPTIONS}
        -Wl,--cref
    )

    target_compile_options(${target} ${visibility} ${COMPILE_OPTIONS})
    target_link_options(${target} ${visibility} ${LINKER_OPTIONS})
endfunction()
