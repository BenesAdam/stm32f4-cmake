include(FetchContent)

#==============================================================================
# Clone libopencm3
#==============================================================================
set(LIBOPENCM3_GIT_URL "https://github.com/libopencm3/libopencm3")
set(LIBOPENCM3_GIT_DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/libopencm3")

# Function for cloning subrepository if needed
function(clone_subrepository URL DIRECTORY)
    if(NOT EXISTS ${DIRECTORY}/.git)
        execute_process(
            COMMAND git clone --config core.autocrlf=false ${URL}
            RESULT_VARIABLE result
        )
        if(result)
            message(FATAL_ERROR "Failed to clone subrepo: ${URL}")
        endif()
    endif()
endfunction()

clone_subrepository(${LIBOPENCM3_GIT_URL} ${LIBOPENCM3_GIT_DESTINATION_DIR})

FetchContent_Declare(
  libopencm3
  SOURCE_DIR ${LIBOPENCM3_GIT_DESTINATION_DIR}
)
FetchContent_MakeAvailable(libopencm3)

#==============================================================================
# Build libopencm3 .a file
#==============================================================================
set(libopencm3_library ${libopencm3_SOURCE_DIR}/lib/libopencm3_stm32f4.a)
set(libopencm3_make_database ${CMAKE_CURRENT_BINARY_DIR}/libopencm3_make_database.txt)

if(NOT EXISTS ${libopencm3_library})
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
  set(libopencm3_targets stm32/f4)
  message(STATUS "Libopencm3 ${libopencm3_targets} building...")
  execute_process(
      COMMAND ${SH_EXECUTABLE} -c 'export PATH=\"${MSYS_PYTHON_DIR}:${MSYS_CMAKE_C_COMPILER_DIR}:\$PATH\" && make -j4 TARGETS=${libopencm3_targets} CC=arm-none-eabi-gcc AR=arm-none-eabi-ar && make TARGETS=${libopencm3_targets} -p > ${libopencm3_make_database}'
      WORKING_DIRECTORY ${libopencm3_SOURCE_DIR}
      RESULT_VARIABLE result
  )
  if(result)
      message(FATAL_ERROR "Failed to build: ${libopencm3_SOURCE_DIR}")
  endif()
  message(STATUS "Libopencm3 ${libopencm3_targets} building - done")
endif()

#==============================================================================
# Remove unused source files
#==============================================================================
# --- Find current dir of libopencm3 target
set(libopencm3_current_dir ${libopencm3_SOURCE_DIR}/lib/stm32/f4/)

if(EXISTS ${libopencm3_make_database})
  # --- Find all object files
  file(STRINGS ${libopencm3_make_database} libopencm3_objects REGEX "OBJS = (.*)")
  string(REPLACE "OBJS = " "" libopencm3_objects ${libopencm3_objects})
  string(REPLACE " " ";" libopencm3_objects ${libopencm3_objects})

  # --- Find dependencies for all object files
  set(LIBOPENCM3_SOURCES)
  foreach(object IN LISTS libopencm3_objects)
    file(STRINGS ${libopencm3_make_database} object_dependencies REGEX "${object}:(.*)")
    string(REGEX MATCHALL "[^ ]+\\.c" object_dependencies ${object_dependencies})
    list(APPEND LIBOPENCM3_SOURCES ${object_dependencies})
  endforeach()

  # --- Transform to absolute paths
  list(REMOVE_DUPLICATES LIBOPENCM3_SOURCES)
  list(TRANSFORM LIBOPENCM3_SOURCES PREPEND ${libopencm3_current_dir})

  foreach(source ${LIBOPENCM3_SOURCES})
      get_filename_component(normalized_source "${source}" REALPATH)
      set(ABSOLUTE_LIBOPENCM3_SOURCES ${ABSOLUTE_LIBOPENCM3_SOURCES} "${normalized_source}")
  endforeach()

  # --- Remove unused files
  file(GLOB_RECURSE libopencm3_unused_source_files "${libopencm3_SOURCE_DIR}/*.c")

  foreach(used_file ${ABSOLUTE_LIBOPENCM3_SOURCES})
    list(FILTER libopencm3_unused_source_files EXCLUDE REGEX "${used_file}$")
  endforeach()

  foreach(unused_file ${libopencm3_unused_source_files})
    file(REMOVE "${unused_file}")
  endforeach()
endif()

#==============================================================================
# Integrate libopencm3 into cmake
#==============================================================================
add_library(stm32 STATIC IMPORTED)
add_dependencies(stm32 libopencm3)

set_property(TARGET stm32 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET stm32 PROPERTY IMPORTED_LOCATION ${libopencm3_library})
target_link_directories(stm32 INTERFACE ${libopencm3_SOURCE_DIR}/lib)

target_compile_definitions(stm32 INTERFACE -DSTM32F4)

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

set(LINKER_OPTIONS
  ${COMPILE_OPTIONS}
  -Wl,-Map=${CMAKE_PROJECT_NAME}.map
  -Wl,--cref
)

target_compile_options(stm32 INTERFACE ${COMPILE_OPTIONS})
target_link_options(stm32 INTERFACE ${LINKER_OPTIONS})
