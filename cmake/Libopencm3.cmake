#==============================================================================
# Clone libopencm3
#==============================================================================
set(LIBOPENCM3_GIT_URL "https://github.com/libopencm3/libopencm3")
set(LIBOPENCM3_RELATIVE_DIR "external/libopencm3")
set(LIBOPENCM3_SOURCE_DIR "${PROJECT_SOURCE_DIR}/${LIBOPENCM3_RELATIVE_DIR}")

# Make sure git is installed on system
find_package(Git REQUIRED)

# Setup directory
make_directory(${LIBOPENCM3_SOURCE_DIR})

# Clone repository
if(NOT EXISTS ${LIBOPENCM3_SOURCE_DIR}/Makefile)
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
set(LIBOPENCM3_MAKE_DATABASE_FILE "libopencm3_make_database.txt")
set(LIBOPENCM3_MAKE_DATABASE "${CMAKE_CURRENT_BINARY_DIR}/${LIBOPENCM3_MAKE_DATABASE_FILE}")
file(RELATIVE_PATH BINARY_RELATIVE_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")


# --- Precompilation settings
set(PRECOMPILED_DIR "${PROJECT_SOURCE_DIR}/precompiled/stm32f4")
set(PRECOMPILED_FILES
    "${LIBOPENCM3_RELATIVE_DIR}/lib/stm32/f4/vector_nvic.c"
    "${LIBOPENCM3_RELATIVE_DIR}/include/libopencmsis/stm32/f4/irqhandlers.h"
    "${LIBOPENCM3_RELATIVE_DIR}/include/libopencm3/stm32/f4/nvic.h"
    "${LIBOPENCM3_RELATIVE_DIR}/lib/libopencm3_stm32f4.a"
)

# --- Load precompilated files
if(ENABLE_LIBOPENCM3_USE_PRECOMPILED)
    message(STATUS "Precompiled libopencm3 files used:")
    # Files from list
    foreach(PRECOMPILED_FILE ${PRECOMPILED_FILES})
        message(STATUS "  * ${PRECOMPILED_FILE}")
        configure_file("${PRECOMPILED_DIR}/${PRECOMPILED_FILE}" "${PROJECT_SOURCE_DIR}/${PRECOMPILED_FILE}" COPYONLY)
    endforeach()

    # Libopencm3 make database
    message(STATUS "  * ${LIBOPENCM3_MAKE_DATABASE_FILE}")
    configure_file("${PRECOMPILED_DIR}/${LIBOPENCM3_MAKE_DATABASE_FILE}" ${LIBOPENCM3_MAKE_DATABASE} COPYONLY)
endif()

# --- Build libopencm3
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

# --- Store precompilated files
if(ENABLE_LIBOPENCM3_STORE_PRECOMPILED)
    message(STATUS "Precompiled libopencm3 files stored:")
    # Files from list
    file(REMOVE_RECURSE ${PRECOMPILED_DIR})
    foreach(PRECOMPILED_FILE ${PRECOMPILED_FILES})
        message(STATUS "  * ${PRECOMPILED_FILE}")
        configure_file("${PROJECT_SOURCE_DIR}/${PRECOMPILED_FILE}" "${PRECOMPILED_DIR}/${PRECOMPILED_FILE}" COPYONLY)
    endforeach()

    # Libopencm3 make database
    message(STATUS "  * ${LIBOPENCM3_MAKE_DATABASE_FILE}")
    configure_file(${LIBOPENCM3_MAKE_DATABASE} "${PRECOMPILED_DIR}/${LIBOPENCM3_MAKE_DATABASE_FILE}" COPYONLY)
endif()

#==============================================================================
# Remove unused source files
#==============================================================================
# --- Find current dir of libopencm3 target
set(LIBOPENCM3_TARGET_OUT_DIR ${LIBOPENCM3_SOURCE_DIR}/lib/stm32/f4/)

if(NOT EXISTS ${LIBOPENCM3_MAKE_DATABASE})
    message(FATAL_ERROR "Libopencm3 make database not exists. (${LIBOPENCM3_MAKE_DATABASE})")
endif()

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

file(REMOVE_RECURSE ${LIBOPENCM3_SOURCE_DIR}/.git)

#==============================================================================
# Integrate libopencm3 into cmake
#==============================================================================
if(${ENABLE_LIBOPENCM3_USE_STATIC})
    # Pros: IntelliSense working.
    # Cons: little bit slower build.
    set(LIBOPENCM3_FILTERED_SOURCES ${ABSOLUTE_LIBOPENCM3_SOURCES})
    list(FILTER LIBOPENCM3_FILTERED_SOURCES EXCLUDE REGEX "vector_nvic\\.c$")
    add_library(libopencm3 STATIC ${LIBOPENCM3_FILTERED_SOURCES})
    target_include_directories(libopencm3 PUBLIC ${LIBOPENCM3_SOURCE_DIR}/include)
    target_compile_definitions(libopencm3 PUBLIC -DSTM32F4)
    target_link_libraries(libopencm3 common)
else()
    # Pros: fast build.
    # Cons: IntelliSense not working on libopencm3 source files.
    add_library(libopencm3 STATIC IMPORTED)
    set_property(TARGET libopencm3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBOPENCM3_SOURCE_DIR}/include)
    set_property(TARGET libopencm3 PROPERTY IMPORTED_LOCATION ${LIBOPENCM3_LIBRARY})
    target_link_directories(libopencm3 INTERFACE ${LIBOPENCM3_SOURCE_DIR}/lib)
    target_compile_definitions(libopencm3 INTERFACE -DSTM32F4)
    include(AddCompileLinkOptions)
    add_compile_link_options(libopencm3 INTERFACE)
endif()
