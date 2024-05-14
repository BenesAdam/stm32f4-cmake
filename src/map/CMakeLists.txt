cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(map)

add_library(map INTERFACE)
target_include_directories(map INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if( CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR )
  add_executable(map_test)
  target_sources(
    map_test PRIVATE 
    const_size_allocator.hpp
    map.hpp
    map_test.cpp)

  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../common ${CMAKE_CURRENT_BINARY_DIR}/common)

  target_link_libraries(map_test PUBLIC map common)
endif()
