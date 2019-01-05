#include "loader.h"

#include <stdlib.h>

#include "exception.h"

void loader_load_arguments(int argc, char *argv[], state_t* state, state_ctx_t* ctx)
{
    argc--;
    argv++;

    switch (argc)
    {
        case 0:
            ctx->graph_file_name = NULL;
            ctx->size = 10;
            ctx->generated = true;
            ctx->threads_nr = 1;
            *state = STATE_SIMPLE_COMPARISON;
            break;

        case 1:
            ctx->graph_file_name = argv[0];
            ctx->generated = false;
            ctx->threads_nr = 4;
            *state = STATE_SIMPLE_COMPARISON;
            break;

        case 4:
        case 5:
            ctx->generated = (bool) atoi(argv[0]);

            if (!ctx->generated && argc == 4) {
                exception_throw(EX_ARGUMENT, "You should provide an input file for graph as a last argument!");
            }

            ctx->size = (size_t) atoi(argv[1]);

            if (!ctx->size) {
                exception_throw(EX_ARGUMENT, "Invalid graph size!");
            }

            ctx->threads_nr = (size_t) atoi(argv[2]);

            if (ctx->threads_nr < 2) {
                exception_throw(EX_ARGUMENT, "Invalid threads number. Should be grater than 1!");
            }

            ctx->solver_type = (solver_type_t) atoi(argv[3]);

            if (ctx->solver_type < SOLVER_SEQUENTIAL || ctx->solver_type >= SOLVER_TOP) {
                exception_throw(EX_ARGUMENT, "Invalid solver type!");
            }

            if (argc == 5) {
                ctx->graph_file_name = argv[4];

                if (!ctx->graph_file_name) {
                    exception_throw(EX_ARGUMENT, "Invalid input graph file name!");
                }
            } else {
                ctx->graph_file_name = NULL;
            };

            *state = STATE_DURATION_TEST;
            break;

        default:
            exception_throw(EX_ARGUMENT, "Invalid program input!");
            break;
    }
}
