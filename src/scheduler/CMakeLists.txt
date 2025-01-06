add_library(scheduler STATIC
    scheduler.cpp
    scheduler.hpp
)

target_include_directories(scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(scheduler common systick)
