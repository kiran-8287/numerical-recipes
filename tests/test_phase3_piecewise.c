#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double square_expr(double x, void *user_data)
{
    (void)user_data;
    return x * x;
}

static double linear_expr(double x, void *user_data)
{
    (void)user_data;
    return 3.0 * x + 1.0;
}

int main(void)
{
    NUMERIX_Function left;
    NUMERIX_Function right;
    NUMERIX_PiecewiseFunction piecewise;
    double result = 0.0;

    numerix_function_init(&left, square_expr, NULL, NULL);
    numerix_function_init(&right, linear_expr, NULL, NULL);

    if (numerix_piecewise_init(&piecewise) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "piecewise init failed\n");
        return 1;
    }

    if (numerix_piecewise_add_segment(&piecewise, 0.0, 2.0, 1, 0, &left) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "failed to add left segment\n");
        numerix_piecewise_destroy(&piecewise);
        return 1;
    }

    if (numerix_piecewise_add_segment(&piecewise, 2.0, 4.0, 1, 1, &right) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "failed to add right segment\n");
        numerix_piecewise_destroy(&piecewise);
        return 1;
    }

    result = numerix_piecewise_evaluate(&piecewise, 1.5);
    if (fabs(result - 2.25) > 1e-12) {
        fprintf(stderr, "left segment evaluation mismatch: %.12f\n", result);
        numerix_piecewise_destroy(&piecewise);
        return 1;
    }

    result = numerix_piecewise_evaluate(&piecewise, 3.0);
    if (fabs(result - 10.0) > 1e-12) {
        fprintf(stderr, "right segment evaluation mismatch: %.12f\n", result);
        numerix_piecewise_destroy(&piecewise);
        return 1;
    }

    if (numerix_piecewise_validate(&piecewise) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "piecewise validation failed\n");
        numerix_piecewise_destroy(&piecewise);
        return 1;
    }

    numerix_piecewise_destroy(&piecewise);
    printf("Phase 3 piecewise engine OK\n");
    return 0;
}
