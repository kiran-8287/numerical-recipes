#include <math.h>
#include <stdlib.h>
#include "numerix.h"

static double numerix_piecewise_segment_value(const NUMERIX_PiecewiseSegment *segment,
                                            double x)
{
    if (segment == NULL || segment->function.evaluate == NULL) {
        return NAN;
    }
    return segment->function.evaluate(x, segment->function.user_data);
}

double numerix_piecewise_solve_root(const NUMERIX_Function *function,
                                    double a,
                                    double b,
                                    double tolerance)
{
    const NUMERIX_PiecewiseFunction *piecewise = NULL;
    size_t i = 0U;

    if (function == NULL || function->type != NUMERIX_FUNCTION_PIECEWISE) {
        return NAN;
    }

    piecewise = (const NUMERIX_PiecewiseFunction *)function->user_data;
    if (piecewise == NULL) {
        return NAN;
    }

    for (i = 0U; i < piecewise->count; ++i) {
        const NUMERIX_PiecewiseSegment *segment = &piecewise->segments[i];
        double left_value = 0.0;
        double right_value = 0.0;

        if (segment->upper_bound < a || segment->lower_bound > b) {
            continue;
        }

        left_value = numerix_piecewise_segment_value(segment, segment->lower_bound);
        right_value = numerix_piecewise_segment_value(segment, segment->upper_bound);

        if (segment->lower_bound == a && left_value == 0.0) {
            return a;
        }
        if (segment->upper_bound == b && right_value == 0.0) {
            return b;
        }
        if (isfinite(left_value) && isfinite(right_value) && left_value * right_value < 0.0) {
            return numerix_bisection(&segment->function,
                                    segment->lower_bound,
                                    segment->upper_bound,
                                    tolerance);
        }
    }

    return NAN;
}
