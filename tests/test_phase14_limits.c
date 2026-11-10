#include <stdio.h>
#include "numerix.h"

int main(void)
{
    NUMERIX_ExecutionLimits limits;

    if (numerix_execution_limits_init(&limits) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "execution limits init failed\n");
        return 1;
    }

    limits.max_iterations = 50U;
    limits.max_function_evaluations = 200U;
    limits.max_execution_time_seconds = 1.0;

    if (!numerix_execution_limits_reached(&limits, 51U, 150U, 0.1)) {
        fprintf(stderr, "iteration limit should be reached\n");
        return 2;
    }

    printf("Phase 14 execution limits OK\n");
    return 0;
}
