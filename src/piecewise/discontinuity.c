#include <math.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_piecewise_detect_discontinuity(const NUMERIX_PiecewiseFunction *piecewise,
                                                    double point,
                                                    NUMERIX_Discontinuity *discontinuity)
{
    size_t i = 0U;
    double left_value = NAN;
    double right_value = NAN;
    int found_left = 0;
    int found_right = 0;

    if (piecewise == NULL || discontinuity == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(discontinuity, 0, sizeof(*discontinuity));
    discontinuity->point = point;
    discontinuity->type = NUMERIX_DISCONTINUITY_NONE;
    strncpy(discontinuity->description, "no discontinuity detected", sizeof(discontinuity->description) - 1U);

    for (i = 0U; i < piecewise->count; ++i) {
        if (piecewise->segments[i].lower_bound == point || piecewise->segments[i].upper_bound == point) {
            if (piecewise->segments[i].function.evaluate != NULL) {
                if (piecewise->segments[i].include_lower && piecewise->segments[i].lower_bound == point) {
                    left_value = piecewise->segments[i].function.evaluate(point, piecewise->segments[i].function.user_data);
                    found_left = 1;
                }
                if (piecewise->segments[i].include_upper && piecewise->segments[i].upper_bound == point) {
                    right_value = piecewise->segments[i].function.evaluate(point, piecewise->segments[i].function.user_data);
                    found_right = 1;
                }
            }
        }
    }

    if (found_left && found_right && isfinite(left_value) && isfinite(right_value) && left_value != right_value) {
        discontinuity->type = NUMERIX_DISCONTINUITY_JUMP;
        strncpy(discontinuity->description, "jump discontinuity at breakpoint", sizeof(discontinuity->description) - 1U);
        return NUMERIX_STATUS_OK;
    }

    if (numerix_piecewise_contains_breakpoint(piecewise, point)) {
        discontinuity->type = NUMERIX_DISCONTINUITY_UNDEFINED;
        strncpy(discontinuity->description, "breakpoint without a defined interval", sizeof(discontinuity->description) - 1U);
        return NUMERIX_STATUS_OK;
    }

    return NUMERIX_STATUS_OK;
}
