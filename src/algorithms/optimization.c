#include <math.h>
#include "numerix.h"

double numerix_golden_section_minimize(const NUMERIX_Function *function,
                                       double a, double b, double tolerance,
                                       size_t max_iterations)
{
    const double ratio = 0.6180339887498948482;
    double width;
    double c;
    double d;
    double fc;
    double fd;
    size_t iteration;

    if (function == NULL || function->evaluate == NULL || !isfinite(a) ||
        !isfinite(b) || !(a < b) || !isfinite(tolerance) || tolerance <= 0.0 ||
        max_iterations == 0U) {
        return NAN;
    }
    width = b - a;
    if (!isfinite(width)) {
        return NAN;
    }
    c = b - ratio * width;
    d = a + ratio * width;
    if (!isfinite(c) || !isfinite(d) || c == d) {
        return NAN;
    }
    fc = numerix_function_evaluate(function, c, NULL);
    fd = numerix_function_evaluate(function, d, NULL);
    if (!isfinite(fc) || !isfinite(fd)) {
        return NAN;
    }

    for (iteration = 0U; iteration < max_iterations; ++iteration) {
        if (b - a <= tolerance) {
            return fc <= fd ? c : d;
        }
        if (fc <= fd) {
            b = d;
            d = c;
            fd = fc;
            c = b - ratio * (b - a);
            if (c == a || c == b || c == d) {
                break;
            }
            fc = numerix_function_evaluate(function, c, NULL);
            if (!isfinite(fc)) {
                return NAN;
            }
        } else {
            a = c;
            c = d;
            fc = fd;
            d = a + ratio * (b - a);
            if (d == a || d == b || d == c) {
                break;
            }
            fd = numerix_function_evaluate(function, d, NULL);
            if (!isfinite(fd)) {
                return NAN;
            }
        }
    }
    if (b - a <= tolerance) {
        return fc <= fd ? c : d;
    }
    return NAN;
}
