#include <math.h>
#include <stdio.h>
#include "numerical_recipes.h"
#include "numerix.h"

double numerix_linear_interpolation(const NUMERIX_Function *function,
                                   double x0,
                                   double y0,
                                   double x1,
                                   double y1,
                                   double x)
{
    (void)function;
    return linear_interpolation(x0, y0, x1, y1, x);
}

double numerix_lagrange_interpolation(const NUMERIX_Function *function,
                                      const double x_values[],
                                      const double y_values[],
                                      size_t n,
                                      double x)
{
    (void)function;
    return lagrange_interpolation(x_values, y_values, n, x);
}

double numerix_newton_interpolation(const NUMERIX_Function *function,
                                    const double x_values[],
                                    const double y_values[],
                                    size_t n,
                                    double x)
{
    (void)function;
    return newton_interpolation(x_values, y_values, n, x);
}
