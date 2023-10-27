cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME cpp-cmake-template)
project(${PROJECT_NAME})
add_subdirectory(src)
include_directories(include)

find_package(GTest REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(tests
    tests/gui/test_gui.cpp
)

target_link_libraries(tests PRIVATE Qt5::Widgets)
add_custom_target(run_tests COMMAND tests)