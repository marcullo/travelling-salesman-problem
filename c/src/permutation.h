#ifndef TSP_SOLVE_PERMUTATION_H
#define TSP_SOLVE_PERMUTATION_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t* sequence;
    size_t n;
} permutation_t;

/*
 * @brief Create permutation dynamically.
 */
permutation_t* permutation_create(size_t first_val, size_t n, size_t skip_val);

/*
 * @brief Deallocate resources if necessary.
 *
 * @param permutation_p - a pointer to pointer to permutation with
 *                        unwanted resources.
 */
void permutation_delete(permutation_t** permutation_p);

/*
 * @brief Reset permutation to the first one.
 */
void permutation_reset(permutation_t* p, size_t first_val, size_t n, size_t skip_val);

/*
 * @brief Get next valid values of permutation in the lexicographical order.
 *
 * @details Null pointer is returned after getting all permutations or in
 *          case of problems.
 */
const size_t* permutation_get_next(permutation_t* p);

#endif //TSP_SOLVE_PERMUTATION_H
