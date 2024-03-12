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

if(NOT EXISTS ${libopencm3_library})
  find_program(SH_EXECUTABLE NAMES sh.exe)
  if(NOT SH_EXECUTABLE)
    message(FATAL_ERROR "sh.exe not found.")
  endif()

  cmake_path(GET CMAKE_C_COMPILER PARENT_PATH CMAKE_C_COMPILER_DIR)
  if(NOT CMAKE_C_COMPILER_DIR)
    message(FATAL_ERROR "CMAKE_C_COMPILER is not set or invalid.")
  endif()

  find_program(PYTHON_EXECUTABLE NAMES python3.exe)
  if(NOT PYTHON_EXECUTABLE)
    message(FATAL_ERROR "python3.exe not found.")
  endif()
  get_filename_component(PYTHON_DIR ${PYTHON_EXECUTABLE} DIRECTORY)

  execute_process(
      COMMAND ${SH_EXECUTABLE} -c 'export PATH=\"${PYTHON_DIR}:${CMAKE_C_COMPILER_DIR}:\$PATH\" && make TARGETS=stm32/f4 CC=arm-none-eabi-gcc AR=arm-none-eabi-ar'
      WORKING_DIRECTORY ${libopencm3_SOURCE_DIR}
      RESULT_VARIABLE result
  )
  if(result)
      message(FATAL_ERROR "Failed to build: ${libopencm3_SOURCE_DIR}")
  endif()
endif()

#==============================================================================
# Integrate libopencm3 into cmake
#==============================================================================
add_library(stm32 STATIC IMPORTED)
set_property(TARGET stm32 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET stm32 PROPERTY IMPORTED_LOCATION ${libopencm3_library})
target_link_directories(stm32 INTERFACE ${libopencm3_SOURCE_DIR}/lib)

file(GLOB_RECURSE LIBOPENCM3_SOURCES "${libopencm3_SOURCE_DIR}/lib/" "*.c")
add_custom_target(libopencm3_sources SOURCES ${LIBOPENCM3_SOURCES})
add_dependencies(stm32 libopencm3_sources)

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

target_compile_options(stm32 INTERFACE ${COMPILE_OPTIONS})
target_link_options(stm32 INTERFACE ${COMPILE_OPTIONS})
