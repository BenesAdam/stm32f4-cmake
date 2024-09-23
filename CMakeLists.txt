cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

project(ProjectStm32 VERSION 0.1.0 LANGUAGES CXX C ASM)

# Variables
#   C/C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#   Options
option(ENABLE_LIBOPENCM3_USE_PRECOMPILED "Whether to use precompiled libopencm3 (make sure you are on disk S:)" OFF)
option(ENABLE_LIBOPENCM3_STORE_PRECOMPILED "Whether to store precompiled libopencm3 (make sure you are on disk S:)" OFF)
option(ENABLE_LIBOPENCM3_USE_STATIC "Whether to use libopencm3 as STATIC library (own compilation in cmake) or IMPORTED library (.a file)." ON)

option(ENABLE_UNIT_TEST "Whether to enable unit testing." OFF)

# Modules
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(CPM)
include(ConfigSafeGuards)
include(AddGitSubmodule)
if(NOT ENABLE_UNIT_TEST)
    include(Libopencm3)
endif()

# Subdirectories
add_subdirectory(src)
add_subdirectory(tools)
if(ENABLE_UNIT_TEST)
    include(CTest)
    enable_testing()    
    add_subdirectory(tests)
else()
    add_subdirectory(app)
endif()
