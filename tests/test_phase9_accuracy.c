#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double root_problem(double x, void *user_data)
{
    (void)user_data;
    return x * x - 4.0;
}

int main(void)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_Method methods[2] = { NUMERIX_METHOD_BISECTION, NUMERIX_METHOD_SECANT };
    double tolerances[5] = { 1e-2, 1e-4, 1e-6, 1e-8, 1e-10 };
    NUMERIX_AccuracyStudy study;

    if (numerix_function_init(&function, root_problem, NULL, NULL) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "function init failed\n");
        return 1;
    }

    problem.function = function;
    problem.x0 = 0.0;
    problem.x1 = 3.0;
    problem.target = 0.0;
    problem.reference_solution = 2.0;
    problem.has_reference_solution = 1;
    problem.has_bracket = 1;
    problem.has_initial_guess = 1;
    problem.preferred_method = NUMERIX_METHOD_BISECTION;
    problem.options.absolute_tolerance = 1e-8;

    if (numerix_accuracy_study(&problem, methods, 2, tolerances, 5, &study) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "accuracy study failed\n");
        return 2;
    }

    if (study.count != 5U) {
        fprintf(stderr, "wrong sample count: %zu\n", study.count);
        return 3;
    }

    if (study.best_error > 1e-6) {
        fprintf(stderr, "best error too large: %.12f\n", study.best_error);
        return 4;
    }

    if (study.summary[0] == '\0') {
        fprintf(stderr, "missing summary\n");
        return 5;
    }

    if (!isfinite(study.best_residual) ||
        !isfinite(study.samples[0].actual_error) ||
        study.samples[0].function_evaluations == 0U) {
        fprintf(stderr, "accuracy study did not retain measured metrics\n");
        return 6;
    }

    printf("Phase 9 accuracy study OK\n");
    return 0;
}
