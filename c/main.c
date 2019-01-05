#include "state_simple_comparison.h"
#include "state_duration_test.h"
#include "loader.h"

int main(int argc, char *argv[])
{
    state_t state;
    state_ctx_t ctx;

    loader_load_arguments(argc, argv, &state, &ctx);

    switch (state)
    {
        case STATE_DURATION_TEST:
            state_duration_test_run(&ctx);
            break;

        case STATE_SIMPLE_COMPARISON:
        default:
            state_simple_comparison_run(&ctx);
            break;
    }

    return 0;
}
