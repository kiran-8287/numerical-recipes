#include <stdio.h>
#include <string.h>
#include "numerix.h"

int main(void)
{
    NUMERIX_ErrorReport report;

    if (numerix_error_report_init(&report) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "error report init failed\n");
        return 1;
    }

    if (numerix_error_detect(NUMERIX_METHOD_NEWTON_RAPHSON,
                            NUMERIX_ERROR_DERIVATIVE_ZERO,
                            "step 4 near x = 1.000000",
                            "The derivative evaluated to approximately zero.",
                            &report) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "error detection failed\n");
        return 2;
    }

    if (report.code != NUMERIX_ERROR_DERIVATIVE_ZERO) {
        fprintf(stderr, "unexpected error code: %d\n", report.code);
        return 3;
    }

    if (strstr(report.what, "derivative") == NULL) {
        fprintf(stderr, "missing human-readable explanation\n");
        return 4;
    }

    if (strstr(report.action, "different initial guess") == NULL &&
        strstr(report.action, "bracket-based") == NULL) {
        fprintf(stderr, "missing suggested action\n");
        return 5;
    }

    printf("Phase 7 error detection OK\n");
    return 0;
}
