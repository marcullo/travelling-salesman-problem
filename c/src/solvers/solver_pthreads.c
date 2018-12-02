#include "solver_pthreads.h"

#include <pthread.h>
#include <stdio.h>
#include <float.h>

#include "permutation.h"
#include "exception.h"

typedef struct {
    size_t id;
    const graph_t* graph;
    path_t* best_path;
} thread_ctx_t;

/*
 * @brief Update best path in case lightweight found.
 *
 * @details The idea is to assign independent permutations of
 * n-2 cities for each thread to process.
 */
void* update_best_path(void* context)
{
    thread_ctx_t* ctx = (thread_ctx_t*) context;
    const graph_t* graph = ctx->graph;
    const size_t n = graph->size;
    const size_t id = ctx->id;

    // There are n - 2 cities left to visit, but each of them should not be 'id'
    permutation_t* p = permutation_create(1, n - 2, id);

    double** costs = graph->costs;
    const size_t* sequence = p->sequence;

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


        if (total_cost < ctx->best_path->cost) {
            ctx->best_path->cost = total_cost;

            ctx->best_path->sequence[0] = 0;
            ctx->best_path->sequence[1] = id;
            for (size_t i = 2; i < n; i++) {
                ctx->best_path->sequence[i] = sequence[i - 2];
            }
            ctx->best_path->sequence[n] = 0;
        }

        sequence = permutation_get_next(p);
    } while (sequence);

    permutation_delete(&p);

    return NULL;
}

/*
 * @brief Solve TSP using pthreads.
 *
 * @details Let's say (without losing generality) that 0 is both begin and end
 *          of a trip. Let's also say that second city is given (corresponding
 *          to the number of thread), equals to id.
 */
bool solver_pthreads_run(const graph_t* graph, path_t* best_path, size_t threads_nr)
{
    if (!graph || !best_path) {
        return false;
    }

    // We need one thread for management
    threads_nr--;

    const size_t n = graph->size;

    const size_t laps = ((n - 1) % threads_nr) ?
            ((n - 1) / threads_nr + 1) : ((n - 1) / threads_nr);

    pthread_t threads[threads_nr];
    thread_ctx_t contexts[threads_nr];

    // Buffer for partial best paths
    path_t* paths[n - 1];
    for (size_t i = 0; i < n - 1; i++) {
        paths[i] = path_create(n + 1);
    }

    for (size_t i = 0; i < laps; i++) {
        // Spawn threads
        for (size_t j = 0; j < threads_nr; j++) {
            const size_t id = i * threads_nr + j + 1;
            if (id > n - 1) {
                break;
            }

            pthread_t* thread = &threads[j];
            thread_ctx_t* ctx = &(contexts[j]);

            ctx->best_path = paths[id - 1];
            ctx->id = id;
            ctx->graph = graph;

            if (pthread_create(thread, NULL, update_best_path, ctx)) {
                exception_throw(EX_THREAD, "Error while creating %d thread at %d lap!", j, i);
            }
        }

        // Join threads
        for (size_t j = 0; j < threads_nr; j++) {
            const size_t id = i * threads_nr + j + 1;
            if (id > n - 1) {
                break;
            }

            if (pthread_join(threads[j], NULL)) {
                exception_throw(EX_THREAD, "Error while joining %d thread at %d lap!", j, i);
            }
        }
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
