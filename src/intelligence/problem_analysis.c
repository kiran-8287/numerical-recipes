#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_problem_summary(const NUMERIX_Problem *problem,
                                      char *summary,
                                      size_t summary_size)
{
    if (problem == NULL || summary == NULL || summary_size == 0U) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    snprintf(summary,
             summary_size,
             "Problem analysis: bracket=%d initial_guess=%d preferred_method=%d",
             problem->has_bracket,
             problem->has_initial_guess,
             problem->preferred_method);
    return NUMERIX_STATUS_OK;
}
