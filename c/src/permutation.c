#include "permutation.h"

#include <stdlib.h>

#include "memory.h"
#include "exception.h"
#include "utils.h"

permutation_t* permutation_create(size_t first_val, size_t n, size_t skip_val)
{
    if (n < 1) {
        exception_throw(EX_ARGUMENT, "Permutation size should be greater or equal to 1");
    }

    permutation_t* permutation = MALLOC(permutation_t, sizeof(permutation_t));
    permutation->sequence = MALLOC_E(size_t, sizeof(size_t) * n, "Error while creating values of permutation");

    permutation_reset(permutation, first_val, n, skip_val);

    return permutation;
}

void permutation_delete(permutation_t** p)
{
    if (!(*p)) {
        return;
    }

    FREE((*p)->sequence);
    FREE(*p);
}

void permutation_reset(permutation_t* p, size_t val, size_t n, size_t skip_val)
{
    if (!p) {
        return;
    }

    if (!p->sequence) {
        return;
    }

    p->n = n;

    for (size_t i = 0; i < n; i++) {
        if (val == skip_val) {
            val++;
        }
        p->sequence[i] = val++;
    }
}

const size_t* permutation_get_next(permutation_t* p)
{
    if (!p) {
        return NULL;
    }

    if (p->n == 1) {
        return NULL;
    }

    size_t* const a = p->sequence;
    const size_t n = p->n;
    size_t i;
    size_t j = n - 1;

    // Find a decrease
    for (i = n - 2; i >= 0; i--) {
        if (a[i] < a[i + 1]) {
            break;
        }

        if (!i) {
            return NULL;
        }
    }

    // Find the greatest value to swap
    while(a[i] >= a[j]) {
        j--;
    }

    swap(&a[i], &a[j]);

    reverse(a + (i + 1), n - (i + 1));

    return p->sequence;
}
