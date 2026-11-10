#include <math.h>
#include <stdio.h>
#include <string.h>
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
    NUMERIX_MethodRecommendation fallback;
    NUMERIX_FallbackHistory history;
    NUMERIX_Status status;

    numerix_function_init(&function, quadratic, NULL, NULL);
    numerix_problem_init(&problem, &function);
    problem.x0 = -2.0;
    problem.x1 = 3.0;
    problem.has_bracket = 1;
    problem.has_initial_guess = 0;

    numerix_fallback_history_init(&history);
    status = numerix_method_fallback(&problem, NUMERIX_METHOD_NEWTON_RAPHSON, &fallback, &history);
    if (status != NUMERIX_STATUS_OK) {
        fprintf(stderr, "fallback failed\n");
        return 1;
    }

    if (fallback.recommended_method != NUMERIX_METHOD_BISECTION) {
        fprintf(stderr, "fallback method mismatch: %d\n", fallback.recommended_method);
        return 1;
    }

    if (history.count != 1U || history.records[0].attempted_method != NUMERIX_METHOD_NEWTON_RAPHSON) {
        fprintf(stderr, "fallback history not preserved\n");
        return 1;
    }

    printf("Phase 5 fallback OK\n");
    return 0;
}
