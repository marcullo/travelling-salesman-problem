#include "memory.h"
#include "exception.h"
#include "state_simple_comparison.h"
#include "state_duration_test.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        exception_throw(EX_ARGUMENT, "You should provide an input file name!");
    }

    state_ctx_t ctx;

    ctx.generated = false;
    ctx.threads_nr = 4;
    ctx.graph_file_name = argv[1];

    state_simple_comparison_run(&ctx);

    ctx.solver_type = SOLVER_SEQUENTIAL;
    state_duration_test_run(&ctx);

    ctx.solver_type = SOLVER_PTHREADS;
    state_duration_test_run(&ctx);

    ctx.solver_type = SOLVER_OPENMP;
    state_duration_test_run(&ctx);

    MEMORY_CHECK();
    return 0;
}
