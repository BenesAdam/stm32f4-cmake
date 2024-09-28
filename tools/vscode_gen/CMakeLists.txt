# Directories
set(VS_CODE_DIR "${PROJECT_SOURCE_DIR}/.vscode")
set(TARGET_CFG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/target_cfg")
set(INTERFACE_CFG_DIR "${CMAKE_CURRENT_SOURCE_DIR}/interface_cfg")
set(SYSTEM_VIEW_DESCRIPTION_DIR "${CMAKE_CURRENT_SOURCE_DIR}/system_view_description")

# Files to generate
set(LAUNCH_FILE "launch.json")
set(C_CPP_PROPERTIES_FILE "c_cpp_properties.json")

# Generate launch.json
set(LAUNCH_DEVICE "${PLATFORM_FULL_NAME_UPPER_CASE}")
set(LAUNCH_EXECUTABLE_FILE "${PROJECT_SOURCE_DIR}/build/app/firmware.elf")
set(LAUNCH_STM_CONFIG_FILE "${TARGET_CFG_DIR}/${PLATFORM_LOWER_CASE}x.cfg") # https://sourceforge.net/p/openocd/code/ci/master/tree/tcl/target/
set(LAUNCH_IFC_CONFIG_FILE "${INTERFACE_CFG_DIR}/stlink.cfg") # https://sourceforge.net/p/openocd/code/ci/master/tree/tcl/interface/
set(LAUNCH_SYSTEM_VIEW_DESCRIPTION_FILE "${SYSTEM_VIEW_DESCRIPTION_DIR}/${PLATFORM_NO_PACKAGE_UPPER_CASE}.svd")
configure_file("${LAUNCH_FILE}.in" "${VS_CODE_DIR}/${LAUNCH_FILE}" @ONLY ESCAPE_QUOTES)

# Generate c_cpp_properties.json
set(C_CPP_PROPERTIES_DEFINITIONS_INDENTATION "                ")
get_target_property(C_CPP_PROPERTIES_DEFINITIONS common INTERFACE_COMPILE_DEFINITIONS)
string(REPLACE ";" "\", \n${C_CPP_PROPERTIES_DEFINITIONS_INDENTATION}\"" C_CPP_PROPERTIES_DEFINITIONS
 "${C_CPP_PROPERTIES_DEFINITIONS_INDENTATION}\"${C_CPP_PROPERTIES_DEFINITIONS}\"")

configure_file("${C_CPP_PROPERTIES_FILE}.in" "${VS_CODE_DIR}/${C_CPP_PROPERTIES_FILE}" @ONLY)
