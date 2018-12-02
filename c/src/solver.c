#include "solver.h"

#include <string.h>
#include <stdlib.h>
#include <float.h>
#include <omp.h>
#include <stdio.h>

#include "memory.h"
#include "exception.h"
#include "solver_seq.h"
#include "solver_pthreads.h"
#include "solver_openmp.h"

static const char* solver_names[] = {
        [SOLVER_SEQUENTIAL] = "SequentialSolver",
        [SOLVER_PTHREADS] = "PthreadsSolver",
        [SOLVER_OPENMP] = "OpenMPSolver",
};

solver_t* solver_create(solver_type_t type, const graph_t* graph)
{
    if (type >= SOLVER_TOP) {
        exception_throw(EX_ARGUMENT, "Invalid solver type!");
    }

    if (!graph) {
        exception_throw(EX_ARGUMENT, "Graph to solve invalid!");
    }

    if (graph->size < 2) {
        exception_throw(EX_ARGUMENT, "Graph size should be equal to 2 minimally");
    }

    const size_t path_len = graph->size + 1;
    solver_t* solver = MALLOC(solver_t, sizeof(solver_t));
    solver->graph = graph;
    solver->solved = false;
    solver->best_path = path_create(path_len);
    memset(solver->best_path->sequence, 0, path_len * sizeof(*(solver->best_path->sequence)));
    solver->best_path->len = path_len;
    solver->best_path->cost = DBL_MAX;

    switch (type)
    {
        case SOLVER_OPENMP:
            solver->solve = &solver_openmp_run;
            break;

        case SOLVER_PTHREADS:
            solver->solve = &solver_pthreads_run;
            break;

        case SOLVER_SEQUENTIAL:
        default:
            solver->solve = &solver_seq_run;
            break;
    }

    solver->type = type;

    return solver;
}

void solver_delete(solver_t** solver)
{
    if (!(*solver)) {
        return;
    }

    path_delete(&(*solver)->best_path);
    FREE(*solver);
}

void solver_run(solver_t* solver)
{
    if (!solver) {
        return;
    }

    if (solver->solve != &solver_seq_run) {
        exception_throw(EX_ARGUMENT, "You cannot run sequential solver using parallel method!");
    }

    solver->start_time = omp_get_wtime();

    solver->solved = solver->solve(solver->graph, solver->best_path, solver->graph->size);

    solver->stop_time = omp_get_wtime();
}

void solver_run_in_parallel(solver_t* solver, size_t threads_nr)
{
    if (!solver) {
        return;
    }

    if (solver->solve == &solver_seq_run) {
        exception_throw(EX_ARGUMENT, "You cannot run parallel solver using sequential method!");
    }

    solver->start_time = omp_get_wtime();

    solver->solved = solver->solve(solver->graph, solver->best_path, threads_nr);

    solver->stop_time = omp_get_wtime();
}

void solver_show(const solver_t* solver)
{
    if (!solver) {
        return;
    }

    const char* name = solver_get_name(solver);

    if (!solver->solved) {
        printf("[%s] Not solved!\n", name);
        return;
    }

    printf("%s\n", name);
    printf("Time: %.6lf s\n", solver->stop_time - solver->start_time);
    printf("Best path: ");
    path_show(solver->best_path);
}

const char* solver_get_name(const solver_t* solver)
{
    if (!solver) {
        return NULL;
    }

    return solver_names[solver->type];
}
