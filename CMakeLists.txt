cmake_minimum_required(VERSION 3.9)
set(TARGET_NAME solver)
project(travelling-salesman-problem C)

find_package(OpenMP REQUIRED)

set(SOURCE_FILES main.c)

set(INCLUDE_DIRS src)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -Wall -Werror")

add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
