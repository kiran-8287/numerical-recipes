#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_reliability_report_init(NUMERIX_ReliabilityReport *report)
{
    if (report == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(report, 0, sizeof(*report));
    report->method = NUMERIX_METHOD_NONE;
    report->status = NUMERIX_STATUS_OK;
    report->value = NAN;
    report->residual = NAN;
    report->estimated_error = NAN;
    report->iterations = 0U;
    report->function_evaluations = 0U;
    report->execution_time_seconds = 0.0;
    report->summary[0] = '\0';
    report->warnings[0] = '\0';
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_reliability_report_from_result(const NUMERIX_Result *result,
                                                    const NUMERIX_Problem *problem,
                                                    NUMERIX_ReliabilityReport *report)
{
    if (result == NULL || report == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_reliability_report_init(report);
    report->method = result->method;
    report->status = result->status;
    report->value = result->value;
    report->residual = result->residual;
    report->estimated_error = result->estimated_error;
    report->iterations = result->iterations;
    report->function_evaluations = result->function_evaluations;
    report->execution_time_seconds = result->execution_time_seconds;

    if (problem != NULL && problem->function.evaluate != NULL &&
        isfinite(report->value)) {
        double eval_at_root = numerix_function_evaluate(&problem->function,
                                                        report->value, NULL);
        if (isfinite(eval_at_root)) {
            report->residual = fabs(eval_at_root);
        }
    }

    if (result->warning[0] != '\0') {
        snprintf(report->warnings,
                 sizeof(report->warnings),
                 "%s",
                 result->warning);
    } else if (!result->converged) {
        snprintf(report->warnings, sizeof(report->warnings),
                 "result is not marked converged");
    } else {
        snprintf(report->warnings, sizeof(report->warnings), "none");
    }

    snprintf(report->summary,
             sizeof(report->summary),
             "method=%d status=%s value=%.12g residual=%.3e iterations=%lu",
             report->method,
             numerix_status_string(report->status),
             report->value,
             report->residual,
             (unsigned long)report->iterations);

    return NUMERIX_STATUS_OK;
}
