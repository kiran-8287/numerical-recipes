#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

#define CHECK_CLOSE(name, actual, expected, tol) \
    do { \
        double err = fabs((actual) - (expected)); \
        if (err <= (tol)) { \
            printf("[PASS] %s\n", name); \
        } else { \
            printf("[FAIL] %s (actual=%.12g expected=%.12g err=%.3e tol=%.3e)\n", \
                   name, (actual), (expected), err, (tol)); \
            return 1; \
        } \
    } while (0)

int main(void)
{
    double result = 0.0;
    double root = 0.0;
    NUMERIX_Function expr;
    NUMERIX_Problem problem;

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "x^2 - 4") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for x^2 - 4\n");
        return 1;
    }

    result = numerix_function_evaluate(&expr, 3.0, NULL);
    CHECK_CLOSE("x^2 - 4 at x=3", result, 5.0, 1e-12);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "sin(x) + cos(x)") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for sin(x) + cos(x)\n");
        return 1;
    }
    result = numerix_function_evaluate(&expr, 0.0, NULL);
    CHECK_CLOSE("sin(x)+cos(x) at x=0", result, 1.0, 1e-12);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "log(x)") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for log(x)\n");
        return 1;
    }
    result = numerix_function_evaluate(&expr, 1.0, NULL);
    CHECK_CLOSE("log(1)", result, 0.0, 1e-12);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "sqrt(9)") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for sqrt(9)\n");
        return 1;
    }
    result = numerix_function_evaluate(&expr, 0.0, NULL);
    CHECK_CLOSE("sqrt(9)", result, 3.0, 1e-12);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "-2^2 + 2^3^2 + 2^-2") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for power precedence expression\n");
        return 1;
    }
    result = numerix_function_evaluate(&expr, 0.0, NULL);
    CHECK_CLOSE("unary and right-associative power precedence", result, 508.25, 1e-12);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "1e-3 + .5") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for scientific notation\n");
        return 1;
    }
    result = numerix_function_evaluate(&expr, 0.0, NULL);
    CHECK_CLOSE("scientific numeric literals", result, 0.501, 1e-12);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "1.2.3") == NUMERIX_STATUS_OK) {
        fprintf(stderr, "malformed numeric literal accepted\n");
        return 1;
    }

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "x^2 - 2") != NUMERIX_STATUS_OK) {
        fprintf(stderr, "parse failed for root expression\n");
        return 1;
    }
    root = numerix_bisection(&expr, 1.0, 2.0, 1e-9);
    CHECK_CLOSE("expression-backed bisection", root, sqrt(2.0), 1e-8);
    CHECK_CLOSE("expression-backed central difference",
                numerix_central_difference(&expr, 3.0, 1e-5), 6.0, 1e-7);
    numerix_function_destroy(&expr);

    numerix_function_init(&expr, NULL, NULL, NULL);
    if (numerix_expression_parse(&expr, "1 / 0") == NUMERIX_STATUS_OK) {
        fprintf(stderr, "division by zero accepted\n");
        return 1;
    }
    if (numerix_problem_init(&problem, &expr) == NUMERIX_STATUS_OK) {
        fprintf(stderr, "problem accepted an invalid expression\n");
        return 1;
    }
    printf("Phase 2 expression engine OK\n");
    return 0;
}
