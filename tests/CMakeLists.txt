CPMAddPackage("gh:mity/acutest#master")
set(ACUTEST_INCLUDE "${Acutest_SOURCE_DIR}/include")

add_executable(ut_map "ut_map.cpp")
target_link_libraries(ut_map PUBLIC map)
target_include_directories(ut_map PRIVATE ${ACUTEST_INCLUDE})

add_test(
    NAME ut_map
    COMMAND $<TARGET_FILE:ut_map>
)
