#ifndef TSP_SOLVE_SOLVER_SEQ_H
#define TSP_SOLVE_SOLVER_SEQ_H

#include <stddef.h>
#include <stdbool.h>

#include "graph.h"
#include "path.h"

bool solver_seq_run(const graph_t* graph, path_t* best_path, size_t n);

#endif //TSP_SOLVE_SOLVER_SEQ_H
