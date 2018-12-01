#include "utils.h"

void swap(size_t* a, size_t* b)
{
    size_t tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse(size_t* arr, size_t size)
{
    for (size_t i = 0; i < size / 2; i++) {
        const size_t tmp = arr[i];
        arr[i] = arr[(size) - (i + 1)];
        arr[size - (i + 1)] = tmp;
    }
}
