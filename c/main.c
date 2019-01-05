#include <stdio.h>
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

    solver_t* solver = solver_create(SOLVER_SEQUENTIAL, graph);
    solver_run(solver);
    solver_show(solver);

    graph_delete(&graph);
    solver_delete(&solver);

    MEMORY_CHECK();
    return 0;
}
