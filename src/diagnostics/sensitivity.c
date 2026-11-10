#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_sensitivity_experiment(const NUMERIX_Problem *problem,
                                             double perturbation,
                                             NUMERIX_SensitivityReport *report)
{
    double delta;
    double shifted_x;
    double base_value;
    double shifted_value;
    double difference;

    if (problem == NULL || report == NULL || problem->function.evaluate == NULL ||
        !isfinite(problem->x0) || !isfinite(perturbation)) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    memset(report, 0, sizeof(*report));
    delta = fabs(perturbation);
    if (delta == 0.0) {
        delta = 1e-6;
    }
    shifted_x = problem->x0 + delta;
    if (!isfinite(shifted_x) || shifted_x == problem->x0) {
        return NUMERIX_STATUS_NUMERICAL_FAILURE;
    }
    base_value = numerix_function_evaluate(&problem->function, problem->x0, NULL);
    shifted_value = numerix_function_evaluate(&problem->function, shifted_x, NULL);
    if (!isfinite(base_value) || !isfinite(shifted_value)) {
        return NUMERIX_STATUS_NONFINITE_RESULT;
    }
    difference = shifted_value - base_value;
    if (!isfinite(difference)) {
        return NUMERIX_STATUS_NUMERICAL_FAILURE;
    }

    report->absolute_sensitivity = fabs(difference);
    report->relative_sensitivity = fabs(base_value) > DBL_MIN ?
        report->absolute_sensitivity / fabs(base_value) : NAN;
    report->perturbation = delta;
    if (isfinite(report->relative_sensitivity)) {
        snprintf(report->summary, sizeof(report->summary),
                 "absolute change %.3e; relative change %.3e at delta %.3e",
                 report->absolute_sensitivity, report->relative_sensitivity, delta);
        return NUMERIX_STATUS_OK;
    }
    snprintf(report->summary, sizeof(report->summary),
             "absolute change %.3e; relative change undefined at zero baseline",
             report->absolute_sensitivity);
    return NUMERIX_STATUS_WARNING;
}
