#ifndef TSP_SOLVE_PATH_H
#define TSP_SOLVE_PATH_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    double cost;
    size_t* sequence;
    size_t len;
} path_t;

/*
 * @brief Create path dynamically.
 */
path_t* path_create(size_t len);

/*
 * @brief Update values of path by these provided by parameters.
 */
void path_update(path_t* path, double cost, const size_t* sequence, size_t len);

/*
 * @brief Deallocate resources if necessary.
 *
 * @param path_p - a pointer to pointer to unwanted resources.
 */
void path_delete(path_t** path_p);

/*
 * @brief Print path: sequence and total cost.
 */
void path_show(const path_t* path);

/*
 * @brief Return true if sequence and costs are equal for given paths.
 */
bool path_compare(const path_t* path1, const path_t* path2);

#endif //TSP_SOLVE_PATH_H
