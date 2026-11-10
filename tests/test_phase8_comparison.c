#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double sample_root(double x, void *user_data)
{
    (void)user_data;
    return x * x - 4.0;
}

int main(void)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_Method methods[3] = {
        NUMERIX_METHOD_BISECTION,
        NUMERIX_METHOD_SECANT,
        NUMERIX_METHOD_NEWTON_RAPHSON
    };
    NUMERIX_ComparisonResult comparison;

    if (numerix_function_init(&function, sample_root, NULL, NULL) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "function init failed\n");
        return 1;
    }

    problem.function = function;
    problem.x0 = 0.0;
    problem.x1 = 3.0;
    problem.target = 0.0;
    problem.has_bracket = 1;
    problem.has_initial_guess = 1;
    problem.preferred_method = NUMERIX_METHOD_BISECTION;
    problem.options.absolute_tolerance = 1e-8;

    if (numerix_compare_methods(&problem, methods, 3, &comparison) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "comparison failed\n");
        return 2;
    }

    if (comparison.count != 3U) {
        fprintf(stderr, "wrong number of compared methods: %zu\n", comparison.count);
        return 3;
    }

    if (!comparison.has_winner) {
        fprintf(stderr, "missing comparison winner\n");
        return 4;
    }

    if (comparison.winner_method != NUMERIX_METHOD_BISECTION &&
        comparison.winner_method != NUMERIX_METHOD_SECANT) {
        fprintf(stderr, "unexpected winner: %d\n", comparison.winner_method);
        return 5;
    }

    if (fabs(comparison.winner_value - 2.0) > 1e-6) {
        fprintf(stderr, "wrong winner value: %.12f\n", comparison.winner_value);
        return 6;
    }

    {
        size_t i;
        int measured_entry_found = 0;
        for (i = 0U; i < comparison.count; ++i) {
            if (comparison.entries[i].converged) {
                measured_entry_found = 1;
                if (!isfinite(comparison.entries[i].residual) ||
                    comparison.entries[i].function_evaluations == 0U ||
                    !isnan(comparison.entries[i].estimated_error)) {
                    fprintf(stderr, "comparison metrics are fabricated or incomplete\n");
                    return 7;
                }
            }
        }
        if (!measured_entry_found) {
            fprintf(stderr, "no measured comparison entry found\n");
            return 8;
        }
    }

    printf("Phase 8 comparison engine OK\n");
    return 0;
}
