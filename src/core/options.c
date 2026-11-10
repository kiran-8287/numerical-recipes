#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_options_init(NUMERIX_Options *options)
{
    if (options == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(options, 0, sizeof(*options));
    options->absolute_tolerance = 1e-8;
    options->relative_tolerance = 1e-8;
    options->max_iterations = 100;
    options->max_execution_time_seconds = 5.0;
    options->derivative_step_size = 1e-6;
    options->diagnostic_level = NUMERIX_DIAGNOSTIC_BASIC;
    options->step_by_step = 0;
    options->enable_fallback = 1;
    options->enable_parallel_execution = 0;

    return NUMERIX_STATUS_OK;
}
