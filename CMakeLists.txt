cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi-gcc.cmake)
set(CMAKE_COLOR_MAKEFILE OFF) # disable color output - not working in vscode output window

project(firmware VERSION 0.1.0 LANGUAGES CXX C ASM)

include(cmake/stm32.cmake)

add_executable(firmware 
  src/main.cpp
  src/types.h
  src/systick.cpp
  src/systick.hpp
  src/i2c1.cpp
  src/i2c1.hpp
  src/display.cpp
  src/display.hpp
)

target_link_libraries(firmware stm32)
set_property(TARGET firmware PROPERTY CXX_STANDARD 17)
stm32_add_linker_script(firmware PRIVATE linker/basic.stm32.ld)
