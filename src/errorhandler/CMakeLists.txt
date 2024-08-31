add_subdirectory(reaction)

add_library(errorhandler STATIC
    errorhandler.cpp
    errorhandler.hpp
    errorhandler_types.cpp
    errorhandler_types.hpp
    errorhandler_inst.cpp
    errorhandler_inst.hpp
)

target_include_directories(errorhandler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(errorhandler error_reaction common)
