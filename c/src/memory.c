#include "memory.h"

#include <stdlib.h>
#include "stdio.h"

#include "exception.h"

unsigned int allocs = 0;
unsigned int allocated = 0;
unsigned int frees = 0;

void* malloc_wrapper(size_t size, const char* parent_function, const char* error_msg)
{
    if (!size) {
        exception_throw(EX_MALLOC, "----- %s: Cannot allocate invalid size 0!", parent_function);
    }

    void* ptr = malloc(size);

    if (!ptr) {
        printf("\n----- ");
        if (error_msg) {
            exception_throw(EX_MALLOC, "%s: %s", parent_function, error_msg);
        } else {
            exception_throw(EX_MALLOC, "%s: Error while allocating memory!", parent_function);
        }
    }

    printf("----- %08x %s: Allocated %lu bytes.\n", (unsigned int) ptr, parent_function, size);
    allocs++;
    allocated += size;

    return ptr;
}

void free_wrapper(void** ptr_p, const char* parent_function)
{
    if (!ptr_p) {
        return;
    }

    if (!(*ptr_p)) {
        return;
    }

    printf("----- %08x %s: Freeing memory.\n", (unsigned int) *ptr_p, parent_function);
    frees++;

    free(*ptr_p);
    *ptr_p = NULL;
}

void memory_check(void)
{
    printf("----- Sum of allocations: %u\n", allocated);

    if (allocs != frees) {
        printf("----- ");
        exception_throw(EX_MALLOC, "Memory leakage!");
    }
}
