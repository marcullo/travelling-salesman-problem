#include <omp.h>

#include "memory.h"
#include "graph.h"
#include "exception.h"
#include "solver.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        exception_throw(EX_ARGUMENT, "You should provide an input file name!");
    }

    graph_t* graph = graph_create_from_file(argv[1]);

    graph_show(graph);

    solver_t* solver_seq = solver_create(SOLVER_SEQUENTIAL, graph);
    solver_run(solver_seq);
    solver_show(solver_seq);

    solver_t* solver_pthreads = solver_create(SOLVER_PTHREADS, graph);
    solver_run_in_parallel(solver_pthreads, 4);
    solver_show(solver_pthreads);

    graph_delete(&graph);
    solver_delete(&solver_seq);
    solver_delete(&solver_pthreads);

    MEMORY_CHECK();
    return 0;
}
