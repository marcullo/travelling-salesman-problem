#ifndef TSP_SOLVE_SOLVER_OPENMP_H
#define TSP_SOLVE_SOLVER_OPENMP_H

#include <stddef.h>

#include "graph.h"
#include "path.h"

bool solver_openmp_run(const graph_t* graph, path_t* best_path, size_t threads_nr);

#endif //TSP_SOLVE_SOLVER_OPENMP_H
