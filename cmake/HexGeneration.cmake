add_custom_command(TARGET ${APP_NAME} POST_BUILD
    COMMENT "Generating HEX from ELF"
    COMMAND ${CMAKE_OBJCOPY}
            -O ihex
            --gap-fill 0xFF
            ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}.elf
            ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}.hex
)

add_custom_command(TARGET ${APP_NAME} POST_BUILD
  COMMAND arm-none-eabi-size ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}.elf
)
