#ifndef TSP_SOLVE_GRAPH_H
#define TSP_SOLVE_GRAPH_H

#include <stdbool.h>
#include <stddef.h>

#define GRAPH_CREATE_RANDOM_MAX_COST 10.0

typedef struct {
    double** costs;
    size_t size;
} graph_t;

/*
 * @brief Generate pseudo-random costs of all routes in the graph.
 */
graph_t* graph_create_random_dbl(size_t size);

/*
 * @brief Generate pseudo-random costs of all routes in the graph.
 *
 * @details Costs will be rounded to int.
 */
graph_t* graph_create_random_int(size_t size);

/*
 * @brief Load graph from file.
 *
 * @details Example:
 *          [in]               [graph size: 3]
 *          3                  [costs]
 *          0.0 100.0 90.0     0<->1: 100.0
 *          100.0 0.0 70.0     0<->2: 90.0
 *          90.0 70.0 0.0      1<->2: 70.0
 *
 *          The first line includes graph size. Next lines include
 *          costs from the vertex (which id corresponds to
 *          line_number - 1) to other vertices (which ids
 *          corresponds to numbers of values within a line).
 *
 *          The double value informs about mentioned cost.
 *          Values are separated by a single space.
 */
graph_t* graph_create_from_file(const char* file_name);

/*
 * @brief Deallocate resources if necessary.
 *
 * @param graph_p - a pointer to pointer to graph with unwanted resources.
 */
void graph_delete(graph_t** graph_p);

/*
 * @brief Print graph cost matrix.
 */
void graph_show(const graph_t* graph);

#endif //TSP_SOLVE_GRAPH_H
