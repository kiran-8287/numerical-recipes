#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double root_case(double x, void *user_data)
{
    (void)user_data;
    return x * x - 9.0;
}

int main(void)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_Method methods[2] = { NUMERIX_METHOD_BISECTION, NUMERIX_METHOD_SECANT };
    NUMERIX_ExternalComparison comparison;

    if (numerix_function_init(&function, root_case, NULL, NULL) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "function init failed\n");
        return 1;
    }

    problem.function = function;
    problem.x0 = 0.0;
    problem.x1 = 4.0;
    problem.target = 0.0;
    problem.reference_solution = 3.0;
    problem.has_reference_solution = 1;
    problem.has_bracket = 1;
    problem.has_initial_guess = 1;
    problem.preferred_method = NUMERIX_METHOD_BISECTION;
    problem.options.absolute_tolerance = 1e-8;

    if (numerix_compare_standard_libraries(&problem, methods, 2, &comparison) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "standard library comparison failed\n");
        return 2;
    }

    if (comparison.count < 1U) {
        fprintf(stderr, "no comparison entries\n");
        return 3;
    }

    if (comparison.summary[0] == '\0') {
        fprintf(stderr, "missing comparison summary\n");
        return 4;
    }

    if (!comparison.has_reference ||
        !isfinite(comparison.entries[0].accuracy) ||
        comparison.entries[0].accuracy > 1e-6 ||
        !isfinite(comparison.entries[0].residual)) {
        fprintf(stderr, "internal reference metrics were not based on the supplied solution\n");
        return 5;
    }
    if (comparison.entries[1].available || !isnan(comparison.entries[1].accuracy)) {
        fprintf(stderr, "unavailable external comparison was reported as measured\n");
        return 6;
    }

    printf("Phase 10 reference comparison OK\n");
    return 0;
}
