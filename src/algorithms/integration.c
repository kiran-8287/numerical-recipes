#include <math.h>
#include "numerix.h"

#define NUMERIX_MAX_INTEGRATION_PANELS 10000000U

static double integration_coordinate(double a, double b, size_t index,
                                     size_t panels)
{
    double fraction = (double)index / (double)panels;
    double difference = b - a;
    if (isfinite(difference)) {
        return a + difference * fraction;
    }
    return a * (1.0 - fraction) + b * fraction;
}

static double compensated_add(double sum, double value, double *correction)
{
    double adjusted = value - *correction;
    double next = sum + adjusted;
    *correction = (next - sum) - adjusted;
    return next;
}

double numerix_trapezoidal_integral(const NUMERIX_Function *function,
                                    double a, double b, size_t panels)
{
    size_t i;
    double step;
    double sum;
    double correction = 0.0;
    double span;

    if (function == NULL || function->evaluate == NULL || !isfinite(a) ||
        !isfinite(b) || panels == 0U || panels > NUMERIX_MAX_INTEGRATION_PANELS) {
        return NAN;
    }
    if (a == b) {
        return 0.0;
    }
    span = b - a;
    step = isfinite(span) ? span / (double)panels :
                             b / (double)panels - a / (double)panels;
    if (!isfinite(step) || step == 0.0) {
        return NAN;
    }
    {
        double fa = numerix_function_evaluate(function, a, NULL);
        double fb = numerix_function_evaluate(function, b, NULL);
        if (!isfinite(fa) || !isfinite(fb)) {
            return NAN;
        }
        sum = 0.5 * fa + 0.5 * fb;
    }
    for (i = 1U; i < panels; ++i) {
        double x = integration_coordinate(a, b, i, panels);
        double fx;
        if (!isfinite(x)) {
            return NAN;
        }
        fx = numerix_function_evaluate(function, x, NULL);
        if (!isfinite(fx)) {
            return NAN;
        }
        sum = compensated_add(sum, fx, &correction);
        if (!isfinite(sum)) {
            return NAN;
        }
    }
    sum *= step;
    return isfinite(sum) ? sum : NAN;
}

double numerix_simpson_integral(const NUMERIX_Function *function,
                                double a, double b, size_t panels)
{
    size_t i;
    double step;
    double sum;
    double correction = 0.0;
    double span;

    if (function == NULL || function->evaluate == NULL || !isfinite(a) ||
        !isfinite(b) || panels < 2U || panels % 2U != 0U ||
        panels > NUMERIX_MAX_INTEGRATION_PANELS) {
        return NAN;
    }
    if (a == b) {
        return 0.0;
    }
    span = b - a;
    step = isfinite(span) ? span / (double)panels :
                             b / (double)panels - a / (double)panels;
    if (!isfinite(step) || step == 0.0) {
        return NAN;
    }
    {
        double fa = numerix_function_evaluate(function, a, NULL);
        double fb = numerix_function_evaluate(function, b, NULL);
        if (!isfinite(fa) || !isfinite(fb)) {
            return NAN;
        }
        sum = fa + fb;
    }
    for (i = 1U; i < panels; ++i) {
        double x = integration_coordinate(a, b, i, panels);
        double fx;
        double weight = (i % 2U == 0U) ? 2.0 : 4.0;
        if (!isfinite(x)) {
            return NAN;
        }
        fx = numerix_function_evaluate(function, x, NULL);
        if (!isfinite(fx)) {
            return NAN;
        }
        sum = compensated_add(sum, weight * fx, &correction);
        if (!isfinite(sum)) {
            return NAN;
        }
    }
    sum *= step / 3.0;
    return isfinite(sum) ? sum : NAN;
}
