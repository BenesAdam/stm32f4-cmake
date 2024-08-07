# Check In-Source build
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(
        FATAL_ERROR
            "In-Source builds not allowed. Please make a build directory.")
endif()

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Debug.")
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Fix PROJECT_SOURCE_DIR
set(PROJECT_SOURCE_DIR_FIXED "${PROJECT_SOURCE_DIR}")
if(PROJECT_SOURCE_DIR_FIXED MATCHES ".*[\\/]$") # if ends with \ or /
    string(REGEX REPLACE "[\\\\/]$" "" PROJECT_SOURCE_DIR_FIXED "${PROJECT_SOURCE_DIR_FIXED}") # remove it
    message(STATUS "Inconsistent project source dir was fixed: ${PROJECT_SOURCE_DIR} -> ${PROJECT_SOURCE_DIR_FIXED}")
endif()
set(PROJECT_SOURCE_DIR ${PROJECT_SOURCE_DIR_FIXED})
