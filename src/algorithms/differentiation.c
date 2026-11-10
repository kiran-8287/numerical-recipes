#include <math.h>
#include "numerix.h"

static double numerix_difference_eval(const NUMERIX_Function *function, double x)
{
    return numerix_function_evaluate(function, x, NULL);
}

double numerix_forward_difference(const NUMERIX_Function *function,
                                  double x,
                                  double h)
{
    double fx;
    double shifted;
    double fshifted;
    double result;

    if (function == NULL || function->evaluate == NULL || !isfinite(x) ||
        !isfinite(h) || h <= 0.0) {
        return NAN;
    }
    shifted = x + h;
    if (!isfinite(shifted) || shifted == x) {
        return NAN;
    }
    fx = numerix_difference_eval(function, x);
    fshifted = numerix_difference_eval(function, shifted);
    if (!isfinite(fx) || !isfinite(fshifted)) {
        return NAN;
    }
    result = (fshifted - fx) / h;
    return isfinite(result) ? result : NAN;
}

double numerix_backward_difference(const NUMERIX_Function *function,
                                   double x,
                                   double h)
{
    double fx;
    double shifted;
    double fshifted;
    double result;

    if (function == NULL || function->evaluate == NULL || !isfinite(x) ||
        !isfinite(h) || h <= 0.0) {
        return NAN;
    }
    shifted = x - h;
    if (!isfinite(shifted) || shifted == x) {
        return NAN;
    }
    fx = numerix_difference_eval(function, x);
    fshifted = numerix_difference_eval(function, shifted);
    if (!isfinite(fx) || !isfinite(fshifted)) {
        return NAN;
    }
    result = (fx - fshifted) / h;
    return isfinite(result) ? result : NAN;
}

double numerix_central_difference(const NUMERIX_Function *function,
                                  double x,
                                  double h)
{
    double left;
    double right;
    double fleft;
    double fright;
    double result;

    if (function == NULL || function->evaluate == NULL || !isfinite(x) ||
        !isfinite(h) || h <= 0.0) {
        return NAN;
    }
    left = x - h;
    right = x + h;
    if (!isfinite(left) || !isfinite(right) || left == x || right == x) {
        return NAN;
    }
    fleft = numerix_difference_eval(function, left);
    fright = numerix_difference_eval(function, right);
    if (!isfinite(fleft) || !isfinite(fright)) {
        return NAN;
    }
    result = ((fright - fleft) / h) * 0.5;
    return isfinite(result) ? result : NAN;
}
