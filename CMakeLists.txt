cmake_minimum_required(VERSION 3.9)

# Project
set(TARGET_NAME solver)
project(travelling-salesman-problem C)

#Package(s)
find_package(OpenMP REQUIRED)

# Source(s)
set(SOURCE_FILES main.c)

# Include(s)
set(INCLUDE_DIRS src)

# Compiler flags and options
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -Wall -Werror -Wno-incompatible-pointer-types -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS_DEBUG  "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# Target(s)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
