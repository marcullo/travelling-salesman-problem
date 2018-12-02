#ifndef TSP_SOLVE_SOLVER_H
#define TSP_SOLVE_SOLVER_H

#include <stdbool.h>

#include "graph.h"
#include "path.h"

typedef enum {
    SOLVER_SEQUENTIAL = 0,
    SOLVER_PTHREADS,
    SOLVER_OPENMP,
    SOLVER_TOP
} solver_type_t;

typedef bool (*solver_if)(const graph_t* graph, path_t* best_path, size_t param);

typedef struct {
    solver_type_t type;
    solver_if solve;
    bool solved;
    path_t* best_path;
    const graph_t* graph;
    double start_time;
    double stop_time;
} solver_t;

/*
 * @brief Create solver for Travelling Salesman Problem.
 */
solver_t* solver_create(solver_type_t type, const graph_t* graph);

/*
 * @brief Delete remaining resources.
 */
void solver_delete(solver_t** solver);

/*
 * @brief Solve problem sequentially.
 */
void solver_run(solver_t* solver);

/*
 * @brief Solve problem in parallel.
 */
void solver_run_in_parallel(solver_t* solver, size_t threads_nr);

/*
 * @brief Print solver results.
 */
void solver_show(const solver_t* solver);

/*
 * @brief Get solver name.
 */
const char* solver_get_name(const solver_t* solver);

#endif //TSP_SOLVE_SOLVER_H
