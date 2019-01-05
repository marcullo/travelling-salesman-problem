#ifndef TSP_SOLVE_STATE_H
#define TSP_SOLVE_STATE_H

#include <stdbool.h>

#include "solver.h"

typedef enum {
    STATE_SIMPLE_COMPARISON = 0,
    STATE_TOP
} state_t;

typedef struct {
    const char* graph_file_name;
    size_t size;
    bool generated;
    size_t threads_nr;
    solver_type_t solver_type;
} state_ctx_t;

#endif //TSP_SOLVE_STATE_H
