#include <stdio.h>
#include "numerix.h"

int main(void)
{
    NUMERIX_TestCase cases[3];
    NUMERIX_TestSuite suite;
    NUMERIX_ValidationSummary summary;
    int i;

    for (i = 0; i < 3; ++i) {
        numerix_test_case_init(&cases[i]);
        cases[i].name = (i == 0) ? "unit" : (i == 1) ? "analytical" : "edge";
        cases[i].category = (i == 0) ? NUMERIX_TEST_UNIT : (i == 1) ? NUMERIX_TEST_ANALYTICAL : NUMERIX_TEST_EDGE_CASE;
        cases[i].passed = 1;
        cases[i].result = NUMERIX_TEST_PASS;
        cases[i].message[0] = '\0';
    }

    numerix_test_suite_init(&suite);
    suite.cases = cases;
    suite.count = 3U;
    suite.name = "phase16_validation";

    if (numerix_validation_run_suite(&suite, &summary) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "validation suite failed\n");
        return 1;
    }

    if (summary.total_tests != 3U || summary.passed_tests != 3U) {
        fprintf(stderr, "unexpected validation totals\n");
        return 2;
    }

    printf("Phase 16 validation suite OK\n");
    return 0;
}
