#include <float.h>
#include <math.h>
#include <stdint.h>
#include "numerix.h"

static int rk4_stage_value(double y, double scale, double dt, double derivative,
                           double *stage)
{
    long double value = (long double)y +
                        (long double)scale * (long double)dt *
                        (long double)derivative;
    if (!isfinite(value) || fabsl(value) > (long double)DBL_MAX) {
        return 0;
    }
    *stage = (double)value;
    return isfinite(*stage);
}

NUMERIX_Status numerix_rk4_solve(NUMERIX_ODECallback derivative,
                                 void *user_data,
                                 double t0, double y0, double t1,
                                 double step_size, size_t max_steps,
                                 double *y_final, size_t *steps_taken)
{
    double distance;
    double required_steps;
    double direction;
    double t;
    double y;
    size_t i;

    if (derivative == NULL || y_final == NULL || !isfinite(t0) ||
        !isfinite(y0) || !isfinite(t1) || !isfinite(step_size) ||
        step_size <= 0.0 || max_steps == 0U) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    if (t0 == t1) {
        *y_final = y0;
        if (steps_taken != NULL) {
            *steps_taken = 0U;
        }
        return NUMERIX_STATUS_OK;
    }
    distance = fabs(t1 - t0);
    if (!isfinite(distance)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    required_steps = ceil(distance / step_size);
    if (!isfinite(required_steps) || required_steps > (double)max_steps ||
        required_steps >= (double)SIZE_MAX) {
        return NUMERIX_STATUS_RESOURCE_LIMIT;
    }
    if (required_steps < 1.0) {
        required_steps = 1.0;
    }
    direction = t1 > t0 ? 1.0 : -1.0;
    t = t0;
    y = y0;

    for (i = 0U; i < (size_t)required_steps; ++i) {
        double remaining = t1 - t;
        double dt = fabs(remaining) <= step_size ? remaining : direction * step_size;
        double k1;
        double k2;
        double k3;
        double k4;
        double y_half1;
        double y_half2;
        double y_full;
        double t_half = t + 0.5 * dt;
        double t_next = (i + 1U == (size_t)required_steps) ? t1 : t + dt;
        long double increment;
        long double next_y;

        if (!isfinite(dt) || dt == 0.0 || !isfinite(t_half) ||
            !isfinite(t_next) || t_next == t) {
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        k1 = derivative(t, y, user_data);
        if (!isfinite(k1)) return NUMERIX_STATUS_NONFINITE_RESULT;
        if (!rk4_stage_value(y, 0.5, dt, k1, &y_half1)) {
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        k2 = derivative(t_half, y_half1, user_data);
        if (!isfinite(k2)) return NUMERIX_STATUS_NONFINITE_RESULT;
        if (!rk4_stage_value(y, 0.5, dt, k2, &y_half2)) {
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        k3 = derivative(t_half, y_half2, user_data);
        if (!isfinite(k3)) return NUMERIX_STATUS_NONFINITE_RESULT;
        if (!rk4_stage_value(y, 1.0, dt, k3, &y_full)) {
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        k4 = derivative(t_next, y_full, user_data);
        if (!isfinite(k4)) return NUMERIX_STATUS_NONFINITE_RESULT;

        increment = ((long double)dt / 6.0L) *
                    ((long double)k1 + 2.0L * (long double)k2 +
                     2.0L * (long double)k3 + (long double)k4);
        next_y = (long double)y + increment;
        if (!isfinite(next_y) || fabsl(next_y) > (long double)DBL_MAX) {
            return NUMERIX_STATUS_NUMERICAL_FAILURE;
        }
        y = (double)next_y;
        t = t_next;
    }

    *y_final = y;
    if (steps_taken != NULL) {
        *steps_taken = (size_t)required_steps;
    }
    return NUMERIX_STATUS_OK;
}
