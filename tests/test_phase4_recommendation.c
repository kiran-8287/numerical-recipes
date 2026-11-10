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
    NUMERIX_MethodRecommendation recommendation;

    numerix_function_init(&function, quadratic, NULL, NULL);
    numerix_problem_init(&problem, &function);
    problem.x0 = -2.0;
    problem.x1 = 3.0;
    problem.has_bracket = 1;
    problem.preferred_method = NUMERIX_METHOD_NONE;

    if (numerix_recommend_method(&problem, &recommendation) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "recommendation failed\n");
        return 1;
    }

    if (recommendation.recommended_method != NUMERIX_METHOD_BISECTION) {
        fprintf(stderr, "wrong recommendation: %d\n", recommendation.recommended_method);
        return 1;
    }

    if (strstr(recommendation.reason, "bracket") == NULL && strstr(recommendation.reason, "sign") == NULL) {
        fprintf(stderr, "missing bracket explanation\n");
        return 1;
    }

    printf("Phase 4 method recommendation OK\n");
    return 0;
}
