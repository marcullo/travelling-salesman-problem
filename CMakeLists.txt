cmake_minimum_required(VERSION 3.9)
include(utils.cmake)

project(travelling-salesman-problem)
print_header()
add_subdirectory(${PROJECT_SOURCE_DIR}/c)
add_subdirectory(${PROJECT_SOURCE_DIR}/cpp)
