include(FetchContent)

set(LIBOPENCM3_GIT_URL "https://github.com/libopencm3/libopencm3")
set(LIBOPENCM3_GIT_DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}/libopencm3")

# Function for cloning subrepository using WSL
function(clone_subrepo_wsl URL DIRECTORY)
    if(NOT EXISTS ${DIRECTORY}/.git)
        execute_process(
            COMMAND wsl --exec git clone ${URL}
            RESULT_VARIABLE result
        )
        if(result)
            message(FATAL_ERROR "Failed to clone subrepo: ${URL}")
        endif()
    endif()
endfunction()

clone_subrepo_wsl(${LIBOPENCM3_GIT_URL} ${LIBOPENCM3_GIT_DESTINATION_DIR})

FetchContent_Declare(
  libopencm3
  SOURCE_DIR ${LIBOPENCM3_GIT_DESTINATION_DIR}
)
FetchContent_MakeAvailable(libopencm3)

# create a target to build libopencm3 -- only for the target we need
add_custom_target(libopencm3 
  COMMAND wsl make TARGETS=stm32/f7 
  WORKING_DIRECTORY ${libopencm3_SOURCE_DIR}
)

# Create a specific CPU target with the appropriate options etc
add_library(stm32f765 STATIC IMPORTED)
set_property(TARGET stm32f765 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET stm32f765 PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_stm32f7.a)
add_dependencies(stm32f765 libopencm3)
target_link_directories(stm32f765 INTERFACE ${libopencm3_SOURCE_DIR}/lib)

target_compile_definitions(stm32f765 INTERFACE -DSTM32F7)

set(COMPILE_OPTIONS 
  --static
  -nostartfiles
  -fno-common
  -mcpu=cortex-m7
  -mthumb
  -mfloat-abi=hard
  -mfpu=fpv5-d16
)
target_compile_options(stm32f765 INTERFACE ${COMPILE_OPTIONS})
target_link_options(stm32f765 INTERFACE ${COMPILE_OPTIONS})


function(stm32_add_flash_targets TARGET)
    add_custom_target(${TARGET}-stlink-flash
      bash -c "openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
                -f /usr/share/openocd/scripts/target/stm32f7x.cfg \
                -c 'reset_config none; program ${TARGET}.elf verify reset exit'"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      DEPENDS ${TARGET}
      VERBATIM
    )
endfunction()