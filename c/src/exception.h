#ifndef TSP_SOLVE_EXCEPTION_H
#define TSP_SOLVE_EXCEPTION_H

#define EXCEPTION_MESSAGE_LEN 100

typedef enum {
    EX_MALLOC = 0,
    EX_FILE_NOT_OPENED,
    EX_ARGUMENT,
    EX_THREAD,
    EX_NULL,
    EX_TOP
} exception_reason_t;

void exception_throw(exception_reason_t reason, const char* msg, ...);

#endif //TSP_SOLVE_EXCEPTION_H
