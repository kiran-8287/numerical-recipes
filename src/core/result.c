#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_result_init(NUMERIX_Result *result)
{
    if (result == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(result, 0, sizeof(*result));
    result->value = 0.0;
    result->residual = 0.0;
    result->estimated_error = 0.0;
    result->iterations = 0U;
    result->function_evaluations = 0U;
    result->execution_time_seconds = 0.0;
    result->converged = 0;
    result->method = NUMERIX_METHOD_NONE;
    result->status = NUMERIX_STATUS_OK;
    result->warning[0] = '\0';
    result->diagnostic[0] = '\0';

    return NUMERIX_STATUS_OK;
}
