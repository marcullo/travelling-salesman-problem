#include <stdio.h>
#include <omp.h>

#include "memory.h"
#include "graph.h"

int main()
{
    graph_t* graph = graph_create_random_dbl(5);

    graph_show(graph);

    graph_delete(&graph);

    MEMORY_CHECK();
    return 0;
}
