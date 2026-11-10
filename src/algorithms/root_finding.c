#include <math.h>
#include "numerix.h"

#define NUMERIX_ROOT_MAX_ITERATIONS 2048U

static int numerix_root_inputs_valid(const NUMERIX_Function *function,
                                     double tolerance)
{
    return function != NULL && function->evaluate != NULL &&
           isfinite(tolerance) && tolerance > 0.0;
}

static double numerix_root_evaluate(const NUMERIX_Function *function, double x)
{
    return numerix_function_evaluate(function, x, NULL);
}

static double numerix_midpoint(double a, double b)
{
    double difference = b - a;
    if (isfinite(difference)) {
        return a + difference * 0.5;
    }
    return a * 0.5 + b * 0.5;
}

double numerix_bisection(const NUMERIX_Function *function,
                         double a,
                         double b,
                         double tolerance)
{
    double fa;
    double fb;
    size_t iteration;

    if (!numerix_root_inputs_valid(function, tolerance) ||
        !isfinite(a) || !isfinite(b) || !(a < b)) {
        return NAN;
    }

    fa = numerix_root_evaluate(function, a);
    fb = numerix_root_evaluate(function, b);
    if (!isfinite(fa) || !isfinite(fb)) {
        return NAN;
    }
    if (fa == 0.0) {
        return a;
    }
    if (fb == 0.0) {
        return b;
    }
    if ((fa < 0.0) == (fb < 0.0)) {
        return NAN;
    }

    for (iteration = 0U; iteration < NUMERIX_ROOT_MAX_ITERATIONS; ++iteration) {
        double midpoint = numerix_midpoint(a, b);
        double fm;

        if (!isfinite(midpoint)) {
            return NAN;
        }
        if (fabs(b * 0.5 - a * 0.5) <= tolerance * 0.5) {
            if (midpoint == a || midpoint == b) {
                return fabs(fa) <= fabs(fb) ? a : b;
            }
        }
        if (midpoint == a || midpoint == b) {
            return NAN;
        }
        fm = numerix_root_evaluate(function, midpoint);
        if (!isfinite(fm)) {
            return NAN;
        }
        if (fm == 0.0 || fabs(b * 0.5 - a * 0.5) <= tolerance * 0.5) {
            return midpoint;
        }
        if ((fa < 0.0) != (fm < 0.0)) {
            b = midpoint;
        } else {
            a = midpoint;
            fa = fm;
        }
    }
    return NAN;
}

double numerix_newton_raphson(const NUMERIX_Function *function,
                              double initial_guess,
                              double tolerance)
{
    double x = initial_guess;
    size_t iteration;

    if (!numerix_root_inputs_valid(function, tolerance) ||
        function->derivative == NULL || !isfinite(x)) {
        return NAN;
    }

    for (iteration = 0U; iteration < NUMERIX_ROOT_MAX_ITERATIONS; ++iteration) {
        double fx = numerix_root_evaluate(function, x);
        double derivative;
        double next;

        if (!isfinite(fx)) {
            return NAN;
        }
        if (fx == 0.0) {
            return x;
        }
        derivative = function->derivative(x, function->user_data);
        if (!isfinite(derivative) || derivative == 0.0) {
            return NAN;
        }
        next = x - fx / derivative;
        if (!isfinite(next)) {
            return NAN;
        }
        if (fabs(next - x) <= tolerance) {
            double residual = numerix_root_evaluate(function, next);
            return isfinite(residual) ? next : NAN;
        }
        x = next;
    }
    return NAN;
}

double numerix_secant(const NUMERIX_Function *function,
                      double x0,
                      double x1,
                      double tolerance)
{
    double f0;
    double f1;
    size_t iteration;

    if (!numerix_root_inputs_valid(function, tolerance) ||
        !isfinite(x0) || !isfinite(x1) || x0 == x1) {
        return NAN;
    }

    f0 = numerix_root_evaluate(function, x0);
    f1 = numerix_root_evaluate(function, x1);
    if (!isfinite(f0) || !isfinite(f1)) {
        return NAN;
    }
    if (f0 == 0.0) {
        return x0;
    }
    if (f1 == 0.0) {
        return x1;
    }

    for (iteration = 0U; iteration < NUMERIX_ROOT_MAX_ITERATIONS; ++iteration) {
        double denominator = f1 - f0;
        double x2;
        double f2;

        if (!isfinite(denominator) || denominator == 0.0) {
            return NAN;
        }
        x2 = x1 - f1 * (x1 - x0) / denominator;
        if (!isfinite(x2)) {
            return NAN;
        }
        f2 = numerix_root_evaluate(function, x2);
        if (!isfinite(f2)) {
            return NAN;
        }
        if (f2 == 0.0 || fabs(x2 - x1) <= tolerance) {
            return x2;
        }
        x0 = x1;
        f0 = f1;
        x1 = x2;
        f1 = f2;
    }
    return NAN;
}
