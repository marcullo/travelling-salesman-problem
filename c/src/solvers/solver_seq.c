#include "solver_seq.h"
#include "permutation.h"

/*
 * @brief Solve TSP sequentially.
 *
 * @details Let's say (without losing generality) that 0 is both begin and
 *          end of a trip. The idea is to find all possible permutations
 *          of remaining n - 1 cities to visit and collect the most
 *          lightweight path.
 */
bool solver_seq_run(const graph_t* graph, path_t* best_path, const size_t n)
{
    if (!graph || !best_path || n < 2) {
        return false;
    }

    // There are n - 1 cities left to visit
    permutation_t* p = permutation_create(1, n - 1, -1);

    double** costs = graph->costs;
    const size_t* sequence = p->sequence;

    do {
        // first is the first city to visit from 0
        const size_t first = sequence[0];

        // last is the last city before going back to 0
        const size_t last = sequence[n - 2];

        double total_cost = costs[0][first];
        for (size_t v = 0; v < n - 2; v++) {
            size_t from = sequence[v];
            size_t to = sequence[v + 1];
            total_cost += costs[from][to];
        }
        total_cost += costs[last][0];

        if (total_cost < best_path->cost) {
            best_path->cost = total_cost;

            best_path->sequence[0] = 0;
            for (size_t i = 1; i < n; i++) {
                best_path->sequence[i] = sequence[i - 1];
            }
            best_path->sequence[n] = 0;
        }

        sequence = permutation_get_next(p);
    } while (sequence);

    permutation_delete(&p);

    return true;
}
