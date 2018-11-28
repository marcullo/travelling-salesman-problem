#include "path.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include "memory.h"
#include "exception.h"

path_t* path_create(size_t len)
{
    if (len < 2) {
        exception_throw(EX_ARGUMENT, "Path vertices nr should be greater or equal to 2");
    }

    path_t* path = MALLOC(path_t, sizeof(path_t));
    path->sequence = MALLOC_E(size_t, sizeof(size_t) * len, "Error while allocating sequence for path");

    path->len = len;
    path->cost = DBL_MAX;

    return path;
}

void path_update(path_t* path, double cost, const size_t* sequence, size_t len)
{
    if (!path || !sequence || len < 2) {
        return;
    }

    path->cost = cost;
    memcpy(path->sequence, sequence, sizeof(size_t) * len);
    path->len= len;
}

void path_delete(path_t** path_p)
{
    if (!path_p) {
        return;
    }

    if (!(*path_p)) {
        return;
    }

    FREE((*path_p)->sequence);
    FREE(*path_p);
}

void path_show(const path_t* path)
{
    if (!path) {
        return;
    }

    if (path->len < 2) {
        return;
    }

    for (size_t i = 0; i < path->len; i++) {
        printf("%lu ", path->sequence[i]);
    } printf("(%.2lf)\n", path->cost);
}

bool path_compare(const path_t* path1, const path_t* path2)
{
    if (!path1 || !path2) {
        return false;
    }
    if (!path1->sequence || !path2->sequence) {
        return false;
    }
    if (path1->cost != path2->cost) {
        return false;
    }
    if (path1->len != path2->len) {
        return false;
    }

    for (size_t i = 0; i < path1->len; i++) {
        if (path1->sequence[i] != path2->sequence[i]) {
            return false;
        }
    }

    return true;
}