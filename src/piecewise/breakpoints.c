#include <stdlib.h>
#include <string.h>
#include "numerix.h"

static int numerix_contains_value(const double *values, size_t count, double x)
{
    size_t i;
    for (i = 0U; i < count; ++i) {
        if (values[i] == x) {
            return 1;
        }
    }
    return 0;
}

NUMERIX_Status numerix_piecewise_collect_breakpoints(const NUMERIX_PiecewiseFunction *piecewise,
                                                    double **breakpoints,
                                                    size_t *count)
{
    size_t i = 0U;
    size_t index = 0U;
    double *values = NULL;

    if (piecewise == NULL || breakpoints == NULL || count == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    values = (double *)malloc((piecewise->count * 2U + 1U) * sizeof(*values));
    if (values == NULL) {
        return NUMERIX_STATUS_ALLOCATION_FAILED;
    }

    for (i = 0U; i < piecewise->count; ++i) {
        if (!numerix_contains_value(values, index, piecewise->segments[i].lower_bound)) {
            values[index++] = piecewise->segments[i].lower_bound;
        }
        if (!numerix_contains_value(values, index, piecewise->segments[i].upper_bound)) {
            values[index++] = piecewise->segments[i].upper_bound;
        }
    }

    *breakpoints = values;
    *count = index;
    return NUMERIX_STATUS_OK;
}

int numerix_piecewise_contains_breakpoint(const NUMERIX_PiecewiseFunction *piecewise,
                                          double x)
{
    size_t i = 0U;

    if (piecewise == NULL) {
        return 0;
    }

    for (i = 0U; i < piecewise->count; ++i) {
        if (piecewise->segments[i].lower_bound == x || piecewise->segments[i].upper_bound == x) {
            return 1;
        }
    }

    return 0;
}
