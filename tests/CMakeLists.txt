# Acutest
CPMAddPackage("gh:mity/acutest#master")
add_library(acutest INTERFACE)
target_include_directories(acutest INTERFACE ${Acutest_SOURCE_DIR}/include)
target_compile_options(acutest INTERFACE -DTEST_NO_EXCEPTIONS)

# Add unit test
macro(add_unit_test library)
    add_test(
        NAME ${library}
        COMMAND $<TARGET_FILE:${library}>
    )    
endmacro()

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src")

# Unit tests
add_subdirectory(bitset)
add_subdirectory(map)
add_subdirectory(systick)
