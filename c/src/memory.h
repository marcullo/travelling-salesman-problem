#ifndef TSP_SOLVE_MEMORY_H
#define TSP_SOLVE_MEMORY_H

#include <stddef.h>
#include <stdlib.h>

// Uncomment to see debug messages of allocation
//#define DEBUG_MEMORY

#ifdef DEBUG_MEMORY
#define MALLOC(type, size) (type*) malloc_wrapper(size, __FUNCTION__, NULL)
#define MALLOC_E(type, size, error_msg) (type*) malloc_wrapper(size, __FUNCTION__, error_msg)
#define FREE(ptr) free_wrapper((void*) &(ptr), __FUNCTION__)
#define MEMORY_CHECK() memory_check()
#else
#define MALLOC(type, size) (type*) malloc(size)
#define MALLOC_E(type, size, error_msg) (type*) malloc(size); (void) error_msg
#define FREE(ptr) free(ptr);
#define MEMORY_CHECK() do {} while(0)
#endif //DEBUG_MEMORY

void* malloc_wrapper(size_t size, const char* parent_function, const char* error_msg);

void free_wrapper(void** ptr_p, const char* parent_function);

void memory_check(void);

#endif //TSP_SOLVE_MEMORY_H
