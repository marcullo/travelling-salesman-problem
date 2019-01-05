cmake_minimum_required(VERSION 3.9)
include(${PROJECT_SOURCE_DIR}/utils.cmake)

# Project
set(TARGET_NAME solver_cpp)
set(LANGUAGES CXX)

# Parameter(s)
#set(AUTOMATIC_VECTORIZE TRUE)

# Source(s)
set(SOURCE_FILES
        main.cpp
)

# Include(s)
set(INCLUDE_DIRS
)

# Compiler flag(s) and option(s)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
set(CMAKE_CXX_FLAGS_DEBUG  "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

if(AUTOMATIC_VECTORIZE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ftree-vectorize")
endif()

# Target(s)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
print_configuration(
        ${TARGET_NAME}
        ${CMAKE_CXX_FLAGS}
        ${CMAKE_CXX_COMPILER_ID}
        ${CMAKE_CXX_COMPILER_VERSION}
        ${LANGUAGES}
        ${CMAKE_CXX_STANDARD}
)

# End
