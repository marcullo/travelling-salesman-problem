#include "graph.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "memory.h"
#include "exception.h"

static graph_t* graph_create(size_t size)
{
    graph_t* graph = MALLOC(graph_t, sizeof(graph_t));

    graph->size = size;
    graph->costs = NULL;

    if (size == 0) {
        return graph;
    }

    graph->costs = MALLOC_E(double**, sizeof(double*) * size, "Error while allocating rows of cost array");

    for (size_t i = 0; i < size; i++) {
        graph->costs[i] = MALLOC_E(double*, sizeof(double) * size, "Error while creating columns of cost array");
    }

    return graph;
}

graph_t* graph_create_random_dbl(size_t size)
{
    graph_t* graph = graph_create(size);
    const double max_cost = GRAPH_CREATE_RANDOM_MAX_COST;
    double cost;

    srand((unsigned int) time(NULL));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= i; j++) {
            if (j == i) {
                cost = 0.0;
            } else {
                cost = (double)rand() / RAND_MAX * max_cost;
            }

            graph->costs[i][j] = cost;
            graph->costs[j][i] = cost;
        }
    }

    return graph;
}

graph_t* graph_create_random_int(size_t size)
{
    graph_t* graph = graph_create(size);
    const double max_cost = GRAPH_CREATE_RANDOM_MAX_COST;
    double cost;

    srand((unsigned int) time(NULL));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= i; j++) {
            if (j == i) {
                cost = 0.0;
            } else {
                cost = (double)rand() / RAND_MAX * max_cost;
            }

            graph->costs[i][j] = (int) cost;
            graph->costs[j][i] = (int) cost;
        }
    }

    return graph;
}

graph_t* graph_create_from_file(const char* file_name)
{
    if (!file_name) {
        exception_throw(EX_NULL, "Invalid file name!");
    }

    FILE* file;
    file = fopen(file_name, "r");

    if (!file) {
        exception_throw(EX_FILE_NOT_OPENED, "Make sure the file exist!");
    }

    size_t size = 0;
    int res = fscanf(file, "%lu", &size);

    if (!size || res == EOF) {
        goto error;
    }

    graph_t* graph = graph_create(size);

    size_t row = 0;
    size_t col = 0;
    char buf[50];

    while (res != EOF) {
        res = fscanf(file, "%s", buf);
        graph->costs[row][col] = atof(buf);
        col++;

        if (col >= size) {
            row++;

            if (row >= size) {
                break;
            }

            col = 0;
        }
    }

    if (row < size || col < size) {
        graph_delete(&graph);
    }

    fclose(file);

    return graph;

error:
    fclose(file);
    exception_throw(EX_NULL, "Error while loading graph from file!");
    return NULL;
}

void graph_delete(graph_t** graph_p)
{
    if (!graph_p) {
        return;
    }

    if (!(*graph_p)) {
        return;
    }

    for (size_t i = 0; i < (*graph_p)->size; i++) {
        FREE((*graph_p)->costs[i]);
    }

    FREE((*graph_p)->costs);
    FREE(*graph_p);
}

void graph_show(const graph_t* const graph)
{
    if (!graph) {
        return;
    }

    const size_t size = graph->size;

    if (!size) {
        return;
    }

    printf("Cost matrix of graph:\n");

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            printf("%.2lf\t", graph->costs[i][j]);
        }
        printf("\n");
    }
}
