include(FetchContent)

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

# create a target to build libopencm3 -- only for the target we need
add_custom_target(libopencm3 
  COMMAND wsl make TARGETS=stm32/f4
  WORKING_DIRECTORY ${libopencm3_SOURCE_DIR}
)

# Create a specific CPU target with the appropriate options etc
add_library(stm32 STATIC IMPORTED)
set_property(TARGET stm32 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${libopencm3_SOURCE_DIR}/include)
set_property(TARGET stm32 PROPERTY IMPORTED_LOCATION ${libopencm3_SOURCE_DIR}/lib/libopencm3_stm32f4.a)
add_dependencies(stm32 libopencm3)
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

target_compile_options(stm32 INTERFACE ${COMPILE_OPTIONS})
target_link_options(stm32 INTERFACE ${COMPILE_OPTIONS})
