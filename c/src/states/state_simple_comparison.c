#include "state_simple_comparison.h"

#include <stdio.h>

#include "memory.h"
#include "exception.h"
#include "solver.h"
#include "graph.h"

static void compare_solutions(const solver_t* reference, solver_t** results, size_t num)
{
    if (!reference || !results || !num) {
        return;
    }
    if (!reference->solved) {
        return;
    }

    const double reference_duration = reference->stop_time - reference->start_time;
    const char* reference_name = solver_get_name(reference);

    double* speedups = MALLOC_E(double, sizeof(double) * num,
            "Error while creating speedups array for solution comparison");

    for (size_t i = 0; i < num; i++) {
        const solver_t* result = results[i];

        if (!reference->solved) {
            continue;
        }

        if (!path_compare(reference->best_path, result->best_path)) {
            printf("Solutions NOT equal.\n");
            return;
        }

        const double result_duration = result->stop_time - result->start_time;
        speedups[i] = reference_duration / result_duration;
    }

    printf("*** All solutions equal.\n");
    printf("%18s: ( reference ) %lf s\n", reference_name, reference_duration);

    for (int i = 0; i < num; i++) {
        const solver_t* result = results[i];

        if (!reference->solved) {
            continue;
        }

        const char* result_name = solver_get_name(result);
        const double result_duration = result->stop_time - result->start_time;

        printf("%18s: %.2lf speedup, %lf s\n", result_name, speedups[i], result_duration);
    }

    FREE(speedups);
}

void state_simple_comparison_run(state_ctx_t* ctx)
{
    const size_t solvers_nr = SOLVER_TOP - SOLVER_SEQUENTIAL;
    solver_t** solvers = MALLOC(solver_t*, sizeof(solver_t*) * solvers_nr);

    graph_t* graph;
    if (!ctx->graph_file_name) {
        graph = graph_create_random_dbl(ctx->size);
    } else {
        graph = graph_create_from_file(ctx->graph_file_name);
    }

    graph_show(graph);

    solvers[0] = solver_create(SOLVER_SEQUENTIAL, graph);
    solver_run(solvers[0]);
    solver_show(solvers[0]);

    for (size_t i = 1; i < SOLVER_TOP; i++) {
        solvers[i] = solver_create((solver_type_t) i, graph);
        solver_run_in_parallel(solvers[i], ctx->threads_nr);
        solver_show(solvers[i]);

    }

    const size_t parallel_solvers_nr = solvers_nr - 1;
    compare_solutions(solvers[0], solvers + 1, parallel_solvers_nr);

    graph_delete(&graph);
    for (size_t i = 0; i < SOLVER_TOP; i++) {
        solver_delete(&solvers[i]);
    }

    FREE(solvers);
}
