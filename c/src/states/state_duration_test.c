#include "state_duration_test.h"

#include <stdio.h>
#include <ctype.h>

#include "state.h"
#include "exception.h"

static void save_results(state_ctx_t* ctx, solver_t* solver)
{
    char out_file_name[60];

    if (solver->type == SOLVER_SEQUENTIAL) {
        sprintf(out_file_name, "C_%s_size_%02lu_slots_%02lu.txt",
                solver_get_name(solver),
                solver->graph->size,
                1);
    } else {
        sprintf(out_file_name, "C_%s_size_%02lu_slots_%02lu.txt",
                solver_get_name(solver),
                solver->graph->size,
                ctx->threads_nr);
    }

    FILE* file;
    if((file = fopen(out_file_name, "r")) != NULL) {
        fclose(file);
    }

    file = fopen(out_file_name, "a");
    if (!file) {
        exception_throw(EX_FILE_NOT_OPENED, "Make sure the file exist!");
    }

    fprintf(file, "%.3lf\n", solver->stop_time - solver->start_time);

    fclose(file);
}

void state_duration_test_run(state_ctx_t *ctx)
{
    graph_t* graph;
    if (!ctx->graph_file_name) {
        graph = graph_create_random_dbl(ctx->size);
    } else {
        graph = graph_create_from_file(ctx->graph_file_name);
    }

    solver_t* solver = solver_create(ctx->solver_type, graph);

    if (solver->type == SOLVER_SEQUENTIAL) {
        solver_run(solver);
    } else {
        solver_run_in_parallel(solver, ctx->threads_nr);
    }

    if (solver->solved) {
        save_results(ctx, solver);
#ifdef DEBUG
        solver_show(solver);
#endif
}

    graph_delete(&graph);
    solver_delete(&solver);

}
