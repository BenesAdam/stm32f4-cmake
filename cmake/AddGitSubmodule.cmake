function(add_git_submodule arg_relative_dir)
    # Make sure git is installed on system
    find_package(Git REQUIRED)

    # Clone and update submodule
    set(FULL_DIR ${PROJECT_SOURCE_DIR}/${arg_relative_dir})

    if(NOT EXISTS ${FULL_DIR}/CMakeLists.txt)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --
                    ${arg_relative_dir} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    endif()

    # Add subdirectory to cmake environment
    if(EXISTS ${FULL_DIR}/CMakeLists.txt)
        message(STATUS "Submodule is CMake Project: ${FULL_DIR}/CMakeLists.txt")
        add_subdirectory(${FULL_DIR})
    else()
        message(STATUS "Submodule is NO CMake Project: ${FULL_DIR}")
    endif()
endfunction()