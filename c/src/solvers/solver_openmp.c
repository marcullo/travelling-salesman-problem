#include "solver_openmp.h"

#include <omp.h>
#include <stdio.h>
#include <float.h>

#include "permutation.h"
#include "exception.h"

/*
 * @brief Solve TSP using pthreads.
 *
 * @details Let's say (without losing generality) that 0 is both begin and end
 *          of a trip. Let's also say that second city is given (corresponding
 *          to the number of thread), equals to id.
 */
bool solver_openmp_run(const graph_t* graph, path_t* best_path, const size_t threads_nr)
{
    if (!graph || !best_path) {
        return false;
    }

    const size_t n = graph->size;
    size_t id;

    const double** costs = graph->costs;

    // Buffer for partial best paths
    path_t* paths[n - 1];
    for (size_t i = 0; i < n - 1; i++) {
        paths[i] = path_create(n + 1);
    }

#pragma omp parallel private(id) shared(costs, paths) num_threads(threads_nr)
#pragma omp for
    for (id = 1; id < n; id++) {
        permutation_t* p = permutation_create(1, n - 2, id);

        const size_t* sequence = p->sequence;
        path_t* path = paths[id - 1];

        do {
            // The first city to visit from id
            const size_t first = sequence[0];

            // The last city before going back to 0
            const size_t last = sequence[n - 3];

            double total_cost = costs[0][id] + costs[id][first];
            for (size_t v = 0; v < n - 3; v++) {
                const size_t from = sequence[v];
                const size_t to = sequence[v + 1];
                total_cost += costs[from][to];
            }
            total_cost += costs[last][0];

            if (total_cost < path->cost) {
                path->cost = total_cost;

                path->sequence[0] = 0;
                path->sequence[1] = id;
                for (size_t i = 2; i < n; i++) {
                    path->sequence[i] = sequence[i - 2];
                }
                path->sequence[n] = 0;
            }

            sequence = permutation_get_next(p);
        } while (sequence);

        permutation_delete(&p);
    }

    for (size_t i = 0; i < n - 1; i++) {
        path_t* path = paths[i];

        // Select best path
        if (path->cost < best_path->cost) {
            best_path->cost = path->cost;

            for (int i = 0; i < path->len; i++) {
                best_path->sequence[i] = path->sequence[i];
            }
        }

        // Delete created content
        path_delete(&path);
    }

    return true;
}
