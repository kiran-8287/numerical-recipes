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
    NUMERIX_Result result;
    NUMERIX_ReliabilityReport report;

    if (numerix_function_init(&function, quadratic, NULL, NULL) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "function init failed\n");
        return 1;
    }

    numerix_problem_init(&problem, &function);
    problem.x0 = 0.0;
    problem.x1 = 3.0;
    problem.has_bracket = 1;
    problem.has_initial_guess = 1;
    problem.preferred_method = NUMERIX_METHOD_BISECTION;
    problem.options.absolute_tolerance = 1e-8;

    numerix_result_init(&result);
    result.value = 2.0;
    result.residual = 1e-10;
    result.estimated_error = 1e-10;
    result.iterations = 10U;
    result.function_evaluations = 25U;
    result.converged = 1;
    result.method = NUMERIX_METHOD_BISECTION;
    result.status = NUMERIX_STATUS_OK;
    snprintf(result.warning, sizeof(result.warning), "none");
    snprintf(result.diagnostic, sizeof(result.diagnostic), "root bracket maintained");

    if (numerix_reliability_report_from_result(&result, &problem, &report) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "reliability report failed\n");
        return 2;
    }

    if (report.summary[0] == '\0') {
        fprintf(stderr, "reliability summary missing\n");
        return 3;
    }

    printf("Phase 11 reliability report OK\n");
    return 0;
}
