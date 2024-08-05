#==============================================================================
# Clone libopencm3
#==============================================================================
set(LIBOPENCM3_GIT_URL "https://github.com/libopencm3/libopencm3")
set(LIBOPENCM3_RELATIVE_DIR "external/libopencm3")
set(LIBOPENCM3_SOURCE_DIR "${CMAKE_SOURCE_DIR}/${LIBOPENCM3_RELATIVE_DIR}")

# Make sure git is installed on system
find_package(Git REQUIRED)

# Setup directory
make_directory(${LIBOPENCM3_SOURCE_DIR})

# Clone repository
if(NOT EXISTS ${LIBOPENCM3_SOURCE_DIR}/.git)
    execute_process(
        COMMAND git clone --config core.autocrlf=false ${LIBOPENCM3_GIT_URL} ${LIBOPENCM3_RELATIVE_DIR}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        RESULT_VARIABLE result
    )

    if(result)
        message(FATAL_ERROR "Failed to clone subrepo: ${URL}")
    endif()
endif()

#==============================================================================
# Build libopencm3 .a file
#==============================================================================
set(LIBOPENCM3_LIBRARY ${LIBOPENCM3_SOURCE_DIR}/lib/libopencm3_stm32f4.a)
set(LIBOPENCM3_MAKE_DATABASE ${CMAKE_CURRENT_BINARY_DIR}/libopencm3_make_database.txt)

if(NOT EXISTS ${LIBOPENCM3_LIBRARY})
    # --- Find MSYS sh.exe
    find_program(SH_EXECUTABLE NAMES sh.exe)
    if(NOT SH_EXECUTABLE)
          message(FATAL_ERROR "MSYS - sh.exe not found.")
    endif()
    message(STATUS "MSYS - sh.exe: ${SH_EXECUTABLE}")

    # --- Set compiler path
    cmake_path(GET CMAKE_C_COMPILER PARENT_PATH CMAKE_C_COMPILER_DIR)
    if(NOT CMAKE_C_COMPILER_DIR)
        message(FATAL_ERROR "MSYS - CMAKE_C_COMPILER is not set or invalid.")
    endif()
    string(REGEX REPLACE "C:/" "/c//" MSYS_CMAKE_C_COMPILER_DIR ${CMAKE_C_COMPILER_DIR})
    message(STATUS "MSYS - CMAKE_C_COMPILER: ${MSYS_CMAKE_C_COMPILER_DIR}")

    # --- Find python3.exe
    find_program(PYTHON_EXECUTABLE NAMES python3.exe)
    if(NOT PYTHON_EXECUTABLE)
        message(FATAL_ERROR "MSYS - python3.exe not found.")
    endif()
    get_filename_component(PYTHON_DIR ${PYTHON_EXECUTABLE} DIRECTORY)
    string(REGEX REPLACE "C:/" "/c//" MSYS_PYTHON_DIR ${PYTHON_DIR})
    message(STATUS "MSYS - python3.exe: ${MSYS_PYTHON_DIR}")

    # --- Build using MSYS shell
    set(LIBOPENCM3_TARGET stm32/f4)
    message(STATUS "Libopencm3 ${LIBOPENCM3_TARGET} building...")
    execute_process(
        COMMAND ${SH_EXECUTABLE} -c 'export PATH=\"${MSYS_PYTHON_DIR}:${MSYS_CMAKE_C_COMPILER_DIR}:\$PATH\" && make -j4 TARGETS=${LIBOPENCM3_TARGET} CC=arm-none-eabi-gcc AR=arm-none-eabi-ar && make TARGETS=${LIBOPENCM3_TARGET} -p > ${LIBOPENCM3_MAKE_DATABASE}'
        WORKING_DIRECTORY ${LIBOPENCM3_SOURCE_DIR}
        RESULT_VARIABLE result
    )
    if(result)
        message(FATAL_ERROR "Failed to build: ${LIBOPENCM3_SOURCE_DIR}")
    endif()
    message(STATUS "Libopencm3 ${LIBOPENCM3_TARGET} building - done")
endif()

#==============================================================================
# Remove unused source files
#==============================================================================
# --- Find current dir of libopencm3 target
set(LIBOPENCM3_TARGET_OUT_DIR ${LIBOPENCM3_SOURCE_DIR}/lib/stm32/f4/)

if(EXISTS ${LIBOPENCM3_MAKE_DATABASE})
    # --- Find all object files
    file(STRINGS ${LIBOPENCM3_MAKE_DATABASE} LIBOPENCM3_OBJECT_FILES REGEX "OBJS = (.*)")
    string(REPLACE "OBJS = " "" LIBOPENCM3_OBJECT_FILES ${LIBOPENCM3_OBJECT_FILES})
    string(REPLACE " " ";" LIBOPENCM3_OBJECT_FILES ${LIBOPENCM3_OBJECT_FILES})

    # --- Find dependencies for all object files
    set(LIBOPENCM3_SOURCES)
    foreach(OBJECT_FILE IN LISTS LIBOPENCM3_OBJECT_FILES)
        file(STRINGS ${LIBOPENCM3_MAKE_DATABASE} OBJECT_FILE_DEPS REGEX "${OBJECT_FILE}:(.*)")
        string(REGEX MATCHALL "[^ ]+\\.c" OBJECT_FILE_DEPS ${OBJECT_FILE_DEPS})
        list(APPEND LIBOPENCM3_SOURCES ${OBJECT_FILE_DEPS})
    endforeach()

    # --- Transform to absolute paths
    list(REMOVE_DUPLICATES LIBOPENCM3_SOURCES)
    list(TRANSFORM LIBOPENCM3_SOURCES PREPEND ${LIBOPENCM3_TARGET_OUT_DIR})

    foreach(SOURCE_FILE ${LIBOPENCM3_SOURCES})
        get_filename_component(SOURCE_FILE_PATH_NORMALIZED "${SOURCE_FILE}" REALPATH)
        set(ABSOLUTE_LIBOPENCM3_SOURCES ${ABSOLUTE_LIBOPENCM3_SOURCES} "${SOURCE_FILE_PATH_NORMALIZED}")
    endforeach()

    # --- Remove unused files
    file(GLOB_RECURSE LIBOPENCM3_UNUSED_SOURCE_FILES "${LIBOPENCM3_SOURCE_DIR}/*.c")

    foreach(used_file ${ABSOLUTE_LIBOPENCM3_SOURCES})
        list(FILTER LIBOPENCM3_UNUSED_SOURCE_FILES EXCLUDE REGEX "${used_file}$")
    endforeach()

    foreach(LIBOPENCM3_UNUSED_SOURCE_FILE ${LIBOPENCM3_UNUSED_SOURCE_FILES})
        file(REMOVE "${LIBOPENCM3_UNUSED_SOURCE_FILE}")
    endforeach()
endif()

#==============================================================================
# Integrate libopencm3 into cmake
#==============================================================================
add_library(libopencm3 STATIC IMPORTED)

set_property(TARGET libopencm3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBOPENCM3_SOURCE_DIR}/include)
set_property(TARGET libopencm3 PROPERTY IMPORTED_LOCATION ${LIBOPENCM3_LIBRARY})
target_link_directories(libopencm3 INTERFACE ${LIBOPENCM3_SOURCE_DIR}/lib)
target_compile_definitions(libopencm3 INTERFACE -DSTM32F4)

set(COMPILE_OPTIONS
    --static
    -nostartfiles
    -fno-common
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -mabi=aapcs
)

# TODO: better way how to say where map file will be located?
set(MAP_FILE ${CMAKE_BINARY_DIR}/app/${CMAKE_PROJECT_NAME}.map)
set(LINKER_OPTIONS
    ${COMPILE_OPTIONS}
    -Wl,-Map=${MAP_FILE}
    -Wl,--cref
)

target_compile_options(libopencm3 INTERFACE ${COMPILE_OPTIONS})
target_link_options(libopencm3 INTERFACE ${LINKER_OPTIONS})