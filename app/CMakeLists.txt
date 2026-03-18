add_executable(${APP_NAME} main.cpp)
target_link_libraries(${APP_NAME}
    libopencm3
    common
    header
)

set(LINKER_SCRIPT_FILE ${PROJECT_SOURCE_DIR}/linker/${PLATFORM_FULL_NAME_UPPER_CASE}.ld)
set(MAP_FILE ${CMAKE_CURRENT_BINARY_DIR}/${APP_NAME}.map)

target_link_options(${APP_NAME} PRIVATE
    -T "${LINKER_SCRIPT_FILE}"
    -Wl,-Map=${MAP_FILE}
)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/HexGeneration.cmake")
