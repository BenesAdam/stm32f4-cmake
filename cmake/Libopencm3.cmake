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
set(LIBOPENCM3_LIBRARY ${LIBOPENCM3_SOURCE_DIR}/lib/libopencm3_${PLATFORM_LOWER_CASE}.a)
set(LIBOPENCM3_MAKE_DATABASE_FILE "libopencm3_make_database.txt")
set(LIBOPENCM3_MAKE_DATABASE "${CMAKE_CURRENT_BINARY_DIR}/${LIBOPENCM3_MAKE_DATABASE_FILE}")
set(LIBOPENCM3_PARSED_SOURCES_FILE "libopencm3_source_files.txt")
set(LIBOPENCM3_PARSED_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${LIBOPENCM3_PARSED_SOURCES_FILE}")
file(RELATIVE_PATH BINARY_RELATIVE_DIR "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}")

# --- Precompilation settings
set(PRECOMPILED_DIR "${PROJECT_SOURCE_DIR}/precompiled/${PLATFORM_LOWER_CASE}")
set(PRECOMPILED_FILES
    "${LIBOPENCM3_RELATIVE_DIR}/lib/${PLATFORM_FOLDER_LOWER_CASE}/vector_nvic.c"
    "${LIBOPENCM3_RELATIVE_DIR}/include/libopencmsis/${PLATFORM_FOLDER_LOWER_CASE}/irqhandlers.h"
    "${LIBOPENCM3_RELATIVE_DIR}/include/libopencm3/${PLATFORM_FOLDER_LOWER_CASE}/nvic.h"
    "${LIBOPENCM3_RELATIVE_DIR}/lib/libopencm3_${PLATFORM_LOWER_CASE}.a"
)

# --- Load precompilated files
if(${ENABLE_LIBOPENCM3_USE_PRECOMPILED})
    message(STATUS "Precompiled libopencm3 files used:")
    # Files from list
    foreach(PRECOMPILED_FILE ${PRECOMPILED_FILES})
        message(STATUS "  * ${PRECOMPILED_FILE}")
        configure_file("${PRECOMPILED_DIR}/${PRECOMPILED_FILE}" "${PROJECT_SOURCE_DIR}/${PRECOMPILED_FILE}" COPYONLY)
    endforeach()

    # Libopencm3 parsed sources
    message(STATUS "  * ${LIBOPENCM3_PARSED_SOURCES_FILE}")
    configure_file("${PRECOMPILED_DIR}/${LIBOPENCM3_PARSED_SOURCES_FILE}" ${LIBOPENCM3_PARSED_SOURCES} COPYONLY)
endif()

# --- Build libopencm3
set(LIBOPENCM3_MAKE_BUILD (${ENABLE_LIBOPENCM3_STORE_PRECOMPILED} OR NOT EXISTS ${LIBOPENCM3_LIBRARY}))
if(${LIBOPENCM3_MAKE_BUILD})
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
    set(LIBOPENCM3_TARGET ${PLATFORM_FOLDER_LOWER_CASE})
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

    # --- Find current dir of libopencm3 target
    message (STATUS "Libopencm3 parsing make database file...")
    set(LIBOPENCM3_TARGET_OUT_DIR ${LIBOPENCM3_SOURCE_DIR}/lib/${PLATFORM_FOLDER_LOWER_CASE}/)

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

    # --- Transform to absolute paths and store it to libopencm3 parsed sources
    list(REMOVE_DUPLICATES LIBOPENCM3_SOURCES)
    list(TRANSFORM LIBOPENCM3_SOURCES PREPEND ${LIBOPENCM3_TARGET_OUT_DIR})
    file(WRITE ${LIBOPENCM3_PARSED_SOURCES} "")

    foreach(SOURCE_FILE ${LIBOPENCM3_SOURCES})
        get_filename_component(SOURCE_FILE_PATH_NORMALIZED "${SOURCE_FILE}" REALPATH)
        file(APPEND ${LIBOPENCM3_PARSED_SOURCES} "${SOURCE_FILE_PATH_NORMALIZED}\n")
    endforeach()
    message (STATUS "Libopencm3 parsing make database file done")

    # --- Store precompilated files
    if(${ENABLE_LIBOPENCM3_STORE_PRECOMPILED})
        message(STATUS "Precompiled libopencm3 files stored:")
        # Files from list
        file(REMOVE_RECURSE ${PRECOMPILED_DIR})
        foreach(PRECOMPILED_FILE ${PRECOMPILED_FILES})
            message(STATUS "  * ${PRECOMPILED_FILE}")
            configure_file("${PROJECT_SOURCE_DIR}/${PRECOMPILED_FILE}" "${PRECOMPILED_DIR}/${PRECOMPILED_FILE}" COPYONLY)
        endforeach()
    
        # Libopencm3 parsed sources
        message(STATUS "  * ${LIBOPENCM3_PARSED_SOURCES_FILE}")
        configure_file(${LIBOPENCM3_PARSED_SOURCES} "${PRECOMPILED_DIR}/${LIBOPENCM3_PARSED_SOURCES_FILE}" COPYONLY)
    endif()
endif()

#==============================================================================
# Load libopencm3 parsed sources
#==============================================================================
if(NOT EXISTS ${LIBOPENCM3_PARSED_SOURCES})
    message(FATAL_ERROR "Libopencm3 parsed sources not exists. (${LIBOPENCM3_PARSED_SOURCES})")
endif()

file(READ ${LIBOPENCM3_PARSED_SOURCES} LIBOPENCM3_SOURCE_FILES)
string(REPLACE "\n" ";" LIBOPENCM3_SOURCE_FILES ${LIBOPENCM3_SOURCE_FILES})

#==============================================================================
# Remove unused source files
#==============================================================================
if(${ENABLE_LIBOPENCM3_REMOVE_UNUSED})
    # --- Remove unused files
    file(GLOB_RECURSE LIBOPENCM3_UNUSED_SOURCE_FILES "${LIBOPENCM3_SOURCE_DIR}/*.c")

    foreach(used_file ${LIBOPENCM3_SOURCE_FILES})
        list(FILTER LIBOPENCM3_UNUSED_SOURCE_FILES EXCLUDE REGEX "${used_file}$")
    endforeach()

    foreach(LIBOPENCM3_UNUSED_SOURCE_FILE ${LIBOPENCM3_UNUSED_SOURCE_FILES})
        file(REMOVE "${LIBOPENCM3_UNUSED_SOURCE_FILE}")
    endforeach()

    # --- Remove unused folders
    file(REMOVE_RECURSE ${LIBOPENCM3_SOURCE_DIR}/.git)

    file(GLOB_RECURSE LIBOPENCM3_LIB_SUBDIRECTORIES LIST_DIRECTORIES true "${LIBOPENCM3_SOURCE_DIR}/lib/*")
    foreach(subdir IN LISTS LIBOPENCM3_LIB_SUBDIRECTORIES)
        if(IS_DIRECTORY "${subdir}")
            file(GLOB content "${subdir}/*")
            set(only_makefile TRUE)
            set(only_subdirs TRUE)

            foreach(item IN LISTS content)
                if(IS_DIRECTORY "${item}")
                    continue()
                elseif(NOT item MATCHES "Makefile")
                    set(only_makefile FALSE)
                    set(only_subdirs FALSE)
                endif()
            endforeach()

            if(only_subdirs OR (only_makefile AND NOT only_subdirs))
                file(REMOVE_RECURSE "${subdir}")
            endif()
        endif()
    endforeach()
endif()

#==============================================================================
# Integrate libopencm3 into cmake
#==============================================================================
if(${ENABLE_LIBOPENCM3_USE_STATIC})
    # Pros: IntelliSense working.
    # Cons: little bit slower build.
    set(LIBOPENCM3_FILTERED_SOURCES ${LIBOPENCM3_SOURCE_FILES})
    list(FILTER LIBOPENCM3_FILTERED_SOURCES EXCLUDE REGEX "vector_nvic\\.c$")
    add_library(libopencm3 STATIC ${LIBOPENCM3_FILTERED_SOURCES})
    target_include_directories(libopencm3 PUBLIC ${LIBOPENCM3_SOURCE_DIR}/include)
    target_link_libraries(libopencm3 common)
else()
    # Pros: fast build.
    # Cons: IntelliSense not working on libopencm3 source files.
    add_library(libopencm3 STATIC IMPORTED)
    set_property(TARGET libopencm3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBOPENCM3_SOURCE_DIR}/include)
    set_property(TARGET libopencm3 PROPERTY IMPORTED_LOCATION ${LIBOPENCM3_LIBRARY})
    target_link_directories(libopencm3 INTERFACE ${LIBOPENCM3_SOURCE_DIR}/lib)
    target_link_libraries(libopencm3 INTERFACE common)
endif()
