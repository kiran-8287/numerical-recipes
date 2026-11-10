#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_execution_limits_init(NUMERIX_ExecutionLimits *limits)
{
    if (limits == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(limits, 0, sizeof(*limits));
    limits->max_iterations = 100U;
    limits->max_function_evaluations = 1000U;
    limits->max_execution_time_seconds = 5.0;
    limits->cancellation_requested = 0;
    limits->reason[0] = '\0';
    return NUMERIX_STATUS_OK;
}

int numerix_execution_limits_reached(const NUMERIX_ExecutionLimits *limits,
                                     size_t iterations,
                                     size_t function_evaluations,
                                     double elapsed_seconds)
{
    if (limits == NULL) {
        return 1;
    }

    if (iterations >= limits->max_iterations) {
        return 1;
    }

    if (function_evaluations >= limits->max_function_evaluations) {
        return 1;
    }

    if (elapsed_seconds >= limits->max_execution_time_seconds) {
        return 1;
    }

    return 0;
}
