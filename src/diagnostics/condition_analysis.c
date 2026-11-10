#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_condition_analysis(const NUMERIX_Problem *problem,
                                        NUMERIX_ConditionReport *report)
{
    double x0;
    double epsilon;
    double x_minus;
    double x_plus;
    double f_minus;
    double f_plus;
    double f0;
    long double derivative;
    long double condition;

    if (problem == NULL || report == NULL || problem->function.evaluate == NULL ||
        !isfinite(problem->x0)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    memset(report, 0, sizeof(*report));
    x0 = problem->x0;
    epsilon = cbrt(DBL_EPSILON) * fmax(1.0, fabs(x0));
    x_minus = x0 - epsilon;
    x_plus = x0 + epsilon;
    if (!isfinite(epsilon) || epsilon <= 0.0 || !isfinite(x_minus) ||
        !isfinite(x_plus) || x_minus == x0 || x_plus == x0) {
        return NUMERIX_STATUS_NUMERICAL_FAILURE;
    }

    f_minus = numerix_function_evaluate(&problem->function, x_minus, NULL);
    f_plus = numerix_function_evaluate(&problem->function, x_plus, NULL);
    f0 = numerix_function_evaluate(&problem->function, x0, NULL);
    if (!isfinite(f_minus) || !isfinite(f_plus) || !isfinite(f0)) {
        return NUMERIX_STATUS_NONFINITE_RESULT;
    }
    derivative = ((long double)f_plus - (long double)f_minus) /
                 ((long double)x_plus - (long double)x_minus);
    if (!isfinite(derivative) || fabsl(derivative) > (long double)DBL_MAX) {
        return NUMERIX_STATUS_NUMERICAL_FAILURE;
    }
    if (f0 == 0.0) {
        report->condition_estimate = INFINITY;
        report->relative_condition = INFINITY;
        report->ill_conditioned = 1;
        snprintf(report->summary, sizeof(report->summary),
                 "relative condition is undefined at a zero function value");
        return NUMERIX_STATUS_WARNING;
    }

    condition = fabsl((long double)x0 * derivative / (long double)f0);
    if (!isfinite(condition) || condition > (long double)DBL_MAX) {
        report->condition_estimate = INFINITY;
        report->relative_condition = INFINITY;
        report->ill_conditioned = 1;
        snprintf(report->summary, sizeof(report->summary),
                 "relative condition exceeds the representable range");
        return NUMERIX_STATUS_WARNING;
    }
    report->condition_estimate = (double)condition;
    report->relative_condition = report->condition_estimate;
    report->ill_conditioned = report->condition_estimate > 1e6;
    snprintf(report->summary, sizeof(report->summary),
             "relative condition %.3e; ill-conditioned=%s",
             report->condition_estimate,
             report->ill_conditioned ? "yes" : "no");
    return NUMERIX_STATUS_OK;
}
