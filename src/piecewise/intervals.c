#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "numerix.h"

static double numerix_piecewise_eval_callback(double x, void *user_data)
{
    return numerix_piecewise_evaluate((const NUMERIX_PiecewiseFunction *)user_data, x);
}

static int numerix_segment_contains_point(const NUMERIX_PiecewiseSegment *segment,
                                          double x)
{
    if (segment == NULL) {
        return 0;
    }

    if (x < segment->lower_bound || x > segment->upper_bound) {
        return 0;
    }

    if (x == segment->lower_bound && !segment->include_lower) {
        return 0;
    }

    if (x == segment->upper_bound && !segment->include_upper) {
        return 0;
    }

    return 1;
}

static int numerix_segment_overlaps(const NUMERIX_PiecewiseSegment *a,
                                   const NUMERIX_PiecewiseSegment *b)
{
    double start = a->lower_bound > b->lower_bound ? a->lower_bound : b->lower_bound;
    double end = a->upper_bound < b->upper_bound ? a->upper_bound : b->upper_bound;
    int a_has_start = a->include_lower;
    int b_has_start = b->include_lower;
    int a_has_end = a->include_upper;
    int b_has_end = b->include_upper;

    if (end < start) {
        return 0;
    }

    if (a->upper_bound == b->lower_bound) {
        return a->include_upper && b->include_lower;
    }

    if (a->lower_bound == b->upper_bound) {
        return a->include_lower && b->include_upper;
    }

    if (start == end) {
        return (a_has_start || b_has_start) && (a_has_end || b_has_end);
    }

    return 1;
}

NUMERIX_Status numerix_piecewise_init(NUMERIX_PiecewiseFunction *piecewise)
{
    if (piecewise == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(piecewise, 0, sizeof(*piecewise));
    piecewise->count = 0U;
    piecewise->capacity = 0U;
    piecewise->has_gaps = 0;
    piecewise->has_overlaps = 0;
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_piecewise_add_segment(NUMERIX_PiecewiseFunction *piecewise,
                                            double lower_bound,
                                            double upper_bound,
                                            int include_lower,
                                            int include_upper,
                                            const NUMERIX_Function *function)
{
    NUMERIX_PiecewiseSegment *segments = NULL;
    size_t new_capacity = 0U;

    if (piecewise == NULL || function == NULL || function->evaluate == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    if (lower_bound > upper_bound) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    if (piecewise->count == piecewise->capacity) {
        new_capacity = piecewise->capacity == 0U ? 4U : piecewise->capacity * 2U;
        segments = (NUMERIX_PiecewiseSegment *)realloc(piecewise->segments,
                                                     new_capacity * sizeof(*segments));
        if (segments == NULL) {
            return NUMERIX_STATUS_ALLOCATION_FAILED;
        }
        piecewise->segments = segments;
        piecewise->capacity = new_capacity;
    }

    piecewise->segments[piecewise->count].lower_bound = lower_bound;
    piecewise->segments[piecewise->count].upper_bound = upper_bound;
    piecewise->segments[piecewise->count].include_lower = include_lower;
    piecewise->segments[piecewise->count].include_upper = include_upper;
    piecewise->segments[piecewise->count].function = *function;
    piecewise->count++;

    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_piecewise_bind(NUMERIX_Function *function,
                                     NUMERIX_PiecewiseFunction *piecewise)
{
    if (function == NULL || piecewise == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    function->type = NUMERIX_FUNCTION_PIECEWISE;
    function->evaluate = numerix_piecewise_eval_callback;
    function->derivative = NULL;
    function->user_data = piecewise;
    function->domain_known = 0;
    function->continuous = 0;
    function->differentiable = 0;
    function->owns_resources = 0;
    return NUMERIX_STATUS_OK;
}

void numerix_piecewise_destroy(NUMERIX_PiecewiseFunction *piecewise)
{
    if (piecewise == NULL) {
        return;
    }

    free(piecewise->segments);
    free(piecewise->breakpoints);
    memset(piecewise, 0, sizeof(*piecewise));
}

NUMERIX_Status numerix_piecewise_validate(const NUMERIX_PiecewiseFunction *piecewise)
{
    size_t i = 0U;
    size_t j = 0U;

    if (piecewise == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    for (i = 0U; i < piecewise->count; ++i) {
        if (piecewise->segments[i].lower_bound > piecewise->segments[i].upper_bound) {
            return NUMERIX_STATUS_INVALID_ARGUMENT;
        }
        if (piecewise->segments[i].lower_bound == piecewise->segments[i].upper_bound &&
            (!piecewise->segments[i].include_lower || !piecewise->segments[i].include_upper)) {
            return NUMERIX_STATUS_INVALID_ARGUMENT;
        }
    }

    for (i = 0U; i < piecewise->count; ++i) {
        for (j = i + 1U; j < piecewise->count; ++j) {
            if (numerix_segment_overlaps(&piecewise->segments[i], &piecewise->segments[j])) {
                return NUMERIX_STATUS_INVALID_ARGUMENT;
            }
        }
    }

    return NUMERIX_STATUS_OK;
}

double numerix_piecewise_evaluate(const NUMERIX_PiecewiseFunction *piecewise,
                                 double x)
{
    size_t i = 0U;

    if (piecewise == NULL) {
        return NAN;
    }

    for (i = 0U; i < piecewise->count; ++i) {
        if (numerix_segment_contains_point(&piecewise->segments[i], x)) {
            if (piecewise->segments[i].function.evaluate != NULL) {
                return piecewise->segments[i].function.evaluate(x,
                                                              piecewise->segments[i].function.user_data);
            }
            return NAN;
        }
    }

    return NAN;
}
