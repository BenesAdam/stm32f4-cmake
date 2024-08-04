cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
set(CMAKE_TOOLCHAIN_FILE cmake/toolchain-arm-none-eabi-gcc.cmake)

project(firmware VERSION 0.1.0 LANGUAGES CXX C ASM)

# Variables
#   Build
set(CMAKE_COLOR_MAKEFILE OFF) # disable color output - not working in vscode output window

#   C/C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(AddGitSubmodule)
include(Libopencm3)

# Subdirectories
add_subdirectory(src)
add_subdirectory(app)
