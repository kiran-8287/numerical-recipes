#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_execution_context_init(NUMERIX_ExecutionContext *context)
{
    if (context == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(context, 0, sizeof(*context));
    numerix_options_init(&context->options);
    context->max_iterations = (int)context->options.max_iterations;
    context->cancelled = 0;
    context->step_by_step = context->options.step_by_step;
    context->last_check_time_seconds = 0.0;

    return NUMERIX_STATUS_OK;
}
