#include <string.h>
#include <stdio.h>
#include "numerix.h"

NUMERIX_Status numerix_test_case_init(NUMERIX_TestCase *test_case)
{
    if (test_case == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(test_case, 0, sizeof(*test_case));
    test_case->name = "unnamed";
    test_case->category = NUMERIX_TEST_UNIT;
    test_case->passed = 0;
    test_case->result = NUMERIX_TEST_FAIL;
    test_case->duration_seconds = 0.0;
    test_case->message[0] = '\0';
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_test_suite_init(NUMERIX_TestSuite *suite)
{
    if (suite == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(suite, 0, sizeof(*suite));
    suite->name = "validation_suite";
    suite->count = 0U;
    suite->has_failures = 0;
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_validation_run_suite(const NUMERIX_TestSuite *suite,
                                          NUMERIX_ValidationSummary *summary)
{
    size_t i;

    if (suite == NULL || summary == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(summary, 0, sizeof(*summary));
    summary->total_tests = suite->count;
    summary->passed_tests = 0U;
    summary->failed_tests = 0U;
    summary->skipped_tests = 0U;

    if (suite->count == 0U) {
        snprintf(summary->summary, sizeof(summary->summary), "no tests configured");
        return NUMERIX_STATUS_OK;
    }

    for (i = 0U; i < suite->count; ++i) {
        const NUMERIX_TestCase *test = &suite->cases[i];

        if (test->passed) {
            summary->passed_tests += 1U;
        } else if (test->result == NUMERIX_TEST_SKIP) {
            summary->skipped_tests += 1U;
        } else {
            summary->failed_tests += 1U;
        }

        summary->total_duration_seconds += test->duration_seconds;
    }

    snprintf(summary->summary,
             sizeof(summary->summary),
             "%s: %lu passed, %lu failed, %lu skipped",
             suite->name != NULL ? suite->name : "validation_suite",
             (unsigned long)summary->passed_tests,
             (unsigned long)summary->failed_tests,
             (unsigned long)summary->skipped_tests);

    return NUMERIX_STATUS_OK;
}
