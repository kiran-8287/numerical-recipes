#include <math.h>
#include <stdio.h>
#include "numerix.h"

static double quadratic(double x, void *user_data)
{
    (void)user_data;
    return x * x - 4.0;
}

int main(void)
{
    NUMERIX_Function function;
    NUMERIX_Problem problem;
    NUMERIX_SensitivityReport sensitivity;
    NUMERIX_ConditionReport condition;

    if (numerix_function_init(&function, quadratic, NULL, NULL) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "function init failed\n");
        return 1;
    }

    numerix_problem_init(&problem, &function);
    problem.x0 = 1.0;
    problem.x1 = 3.0;
    problem.has_bracket = 1;
    problem.preferred_method = NUMERIX_METHOD_BISECTION;
    problem.options.absolute_tolerance = 1e-8;

    if (numerix_sensitivity_experiment(&problem, 1e-4, &sensitivity) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "sensitivity failed\n");
        return 2;
    }

    if (numerix_condition_analysis(&problem, &condition) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "condition analysis failed\n");
        return 3;
    }

    if (sensitivity.summary[0] == '\0' || condition.summary[0] == '\0') {
        fprintf(stderr, "missing sensitivity summary\n");
        return 4;
    }
    if (fabs(condition.condition_estimate - (2.0 / 3.0)) > 1e-6 ||
        fabs(sensitivity.absolute_sensitivity - 2.0001e-4) > 1e-8 ||
        !isfinite(sensitivity.relative_sensitivity)) {
        fprintf(stderr, "sensitivity or relative-condition result is inaccurate\n");
        return 5;
    }

    printf("Phase 13 sensitivity analysis OK\n");
    return 0;
}
