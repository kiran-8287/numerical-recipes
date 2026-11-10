#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

static int numerix_record_candidate(NUMERIX_PortfolioResult *result,
                                   NUMERIX_Method method,
                                   double value,
                                   double residual,
                                   int verified,
                                   const char *diagnostic)
{
    if (result == NULL || result->count >= 8U) {
        return 0;
    }

    result->candidates[result->count].method = method;
    result->candidates[result->count].value = value;
    result->candidates[result->count].residual = residual;
    result->candidates[result->count].verified = verified;
    result->candidates[result->count].execution_time_seconds = 0.0;
    if (diagnostic != NULL) {
        snprintf(result->candidates[result->count].diagnostic,
                 sizeof(result->candidates[result->count].diagnostic),
                 "%s",
                 diagnostic);
    } else {
        result->candidates[result->count].diagnostic[0] = '\0';
    }
    result->count++;
    return 1;
}

static void numerix_select_winner(NUMERIX_PortfolioResult *result)
{
    size_t i = 0U;
    size_t best_index = 0U;

    if (result == NULL || result->count == 0U) {
        return;
    }

    for (i = 1U; i < result->count; ++i) {
        if (result->candidates[i].verified &&
            (!result->candidates[best_index].verified ||
             result->candidates[i].residual < result->candidates[best_index].residual)) {
            best_index = i;
        }
    }

    if (!result->candidates[best_index].verified) {
        return;
    }

    result->winner = result->candidates[best_index].method;
    result->winner_value = result->candidates[best_index].value;
    result->winner_residual = result->candidates[best_index].residual;
    result->has_winner = 1;
    snprintf(result->summary,
             sizeof(result->summary),
             "winner=%d value=%.12g residual=%.3e",
             result->winner,
             result->winner_value,
             result->winner_residual);
}

NUMERIX_Status numerix_portfolio_solve(const NUMERIX_Problem *problem,
                                      NUMERIX_PortfolioResult *result)
{
    double tol = 1e-8;
    double bisect_value = NAN;
    double secant_value = NAN;
    double newton_value = NAN;
    double residual = 0.0;

    if (problem == NULL || result == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(result, 0, sizeof(*result));
    if (problem->function.evaluate == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    if (problem->options.absolute_tolerance > 0.0) {
        tol = problem->options.absolute_tolerance;
    }

    if (problem->has_bracket) {
        bisect_value = numerix_bisection(&problem->function, problem->x0, problem->x1, tol);
        residual = fabs(problem->function.evaluate(bisect_value, problem->function.user_data));
        numerix_record_candidate(result,
                                NUMERIX_METHOD_BISECTION,
                                bisect_value,
                                residual,
                                isfinite(bisect_value) && residual <= tol * 10.0,
                                "bracket-preserving method");

        secant_value = numerix_secant(&problem->function, problem->x0, problem->x1, tol);
        residual = fabs(problem->function.evaluate(secant_value, problem->function.user_data));
        numerix_record_candidate(result,
                                NUMERIX_METHOD_SECANT,
                                secant_value,
                                residual,
                                isfinite(secant_value) && residual <= tol * 10.0,
                                "secant estimate");
    }

    if (problem->has_initial_guess && problem->function.derivative != NULL) {
        newton_value = numerix_newton_raphson(&problem->function, problem->x0, tol);
        residual = fabs(problem->function.evaluate(newton_value, problem->function.user_data));
        numerix_record_candidate(result,
                                NUMERIX_METHOD_NEWTON_RAPHSON,
                                newton_value,
                                residual,
                                isfinite(newton_value) && residual <= tol * 10.0,
                                "Newton-Raphson estimate");
    }

    if (result->count == 0U) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_select_winner(result);
    if (!result->has_winner) {
        result->winner = NUMERIX_METHOD_NONE;
        result->winner_value = NAN;
        result->winner_residual = NAN;
        snprintf(result->summary, sizeof(result->summary), "no verified winner available");
        return NUMERIX_STATUS_CONVERGENCE_FAILURE;
    }

    return NUMERIX_STATUS_OK;
}
