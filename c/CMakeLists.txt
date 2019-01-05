cmake_minimum_required(VERSION 3.9)
include(${PROJECT_SOURCE_DIR}/utils.cmake)

# Project
set(TARGET_NAME solver_c)
set(LANGUAGES C)

#Package(s)
find_package(OpenMP REQUIRED)

# Parameters
#set(AUTOMATIC_VECTORIZE TRUE)
#set(DEBUG_MEMORY TRUE)

# Source(s)
set(SOURCE_FILES
        main.c
        src/memory.c
        src/exception.c
        src/graph.c
        src/path.c
        src/utils.c
        src/permutation.c
        src/loader.c
        src/solver.c
        src/solvers/solver_seq.c
        src/solvers/solver_pthreads.c
        src/solvers/solver_openmp.c
        src/states/state_simple_comparison.c
        src/states/state_duration_test.c
)

# Include(s)
set(INCLUDE_DIRS
        src
        src/solvers
        src/states
)

# Compiler flag(s) and option(s)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -Wall -Werror -Wno-format -Wno-incompatible-pointer-types -Wno-pointer-to-int-cast")
set(CMAKE_C_FLAGS_DEBUG  "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

if(DEBUG_MEMORY)
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG_MEMORY")
endif()

if(AUTOMATIC_VECTORIZE)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ftree-vectorize")
endif()

# Target(s)
add_executable(${TARGET_NAME} ${SOURCE_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${INCLUDE_DIRS})
print_configuration(
        ${TARGET_NAME}
        ${CMAKE_C_FLAGS}
        ${CMAKE_C_COMPILER_ID}
        ${CMAKE_C_COMPILER_VERSION}
        ${LANGUAGES}
        ${CMAKE_C_STANDARD}
)

# End
