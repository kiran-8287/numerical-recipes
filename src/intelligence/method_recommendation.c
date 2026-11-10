#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_recommendation_init(NUMERIX_MethodRecommendation *recommendation)
{
    if (recommendation == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(recommendation, 0, sizeof(*recommendation));
    recommendation->recommended_method = NUMERIX_METHOD_NONE;
    recommendation->confidence = 0;
    snprintf(recommendation->reason, sizeof(recommendation->reason), "no recommendation available");
    snprintf(recommendation->advantages, sizeof(recommendation->advantages), "none");
    snprintf(recommendation->limitations, sizeof(recommendation->limitations), "none");
    snprintf(recommendation->alternatives, sizeof(recommendation->alternatives), "none");
    return NUMERIX_STATUS_OK;
}

static int numerix_bracket_is_valid(const NUMERIX_Problem *problem)
{
    double left_value = 0.0;
    double right_value = 0.0;

    if (problem == NULL || problem->function.evaluate == NULL || !problem->has_bracket) {
        return 0;
    }

    left_value = problem->function.evaluate(problem->x0, problem->function.user_data);
    right_value = problem->function.evaluate(problem->x1, problem->function.user_data);

    if (!isfinite(left_value) || !isfinite(right_value)) {
        return 0;
    }

    if (left_value == 0.0 || right_value == 0.0) {
        return 1;
    }

    return left_value * right_value < 0.0;
}

NUMERIX_Status numerix_recommend_method(const NUMERIX_Problem *problem,
                                       NUMERIX_MethodRecommendation *recommendation)
{
    if (problem == NULL || recommendation == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_recommendation_init(recommendation);

    if (numerix_bracket_is_valid(problem)) {
        recommendation->recommended_method = NUMERIX_METHOD_BISECTION;
        recommendation->confidence = 90;
        snprintf(recommendation->reason,
                 sizeof(recommendation->reason),
                 "A valid sign-changing bracket was found for a reliable root solve.");
        snprintf(recommendation->advantages,
                 sizeof(recommendation->advantages),
                 "Bracket-preserving convergence and strong reliability.");
        snprintf(recommendation->limitations,
                 sizeof(recommendation->limitations),
                 "Slower than Newton-Raphson in well-behaved smooth problems.");
        snprintf(recommendation->alternatives,
                 sizeof(recommendation->alternatives),
                 "Secant Method or Newton-Raphson when derivative information is available.");
        return NUMERIX_STATUS_OK;
    }

    if (problem->has_initial_guess && problem->function.derivative != NULL) {
        recommendation->recommended_method = NUMERIX_METHOD_NEWTON_RAPHSON;
        recommendation->confidence = 75;
        snprintf(recommendation->reason,
                 sizeof(recommendation->reason),
                 "A usable initial guess and derivative are available for Newton-Raphson.");
        snprintf(recommendation->advantages,
                 sizeof(recommendation->advantages),
                 "Fast local convergence when the derivative is reliable.");
        snprintf(recommendation->limitations,
                 sizeof(recommendation->limitations),
                 "Can fail if the derivative is zero, unstable, or the initial guess is poor.");
        snprintf(recommendation->alternatives,
                 sizeof(recommendation->alternatives),
                 "Bisection or Secant Method.");
        return NUMERIX_STATUS_OK;
    }

    if (problem->has_initial_guess) {
        recommendation->recommended_method = NUMERIX_METHOD_SECANT;
        recommendation->confidence = 65;
        snprintf(recommendation->reason,
                 sizeof(recommendation->reason),
                 "A suitable initial estimate exists, but derivative information is unavailable.");
        snprintf(recommendation->advantages,
                 sizeof(recommendation->advantages),
                 "Derivative-free local convergence for many smooth problems.");
        snprintf(recommendation->limitations,
                 sizeof(recommendation->limitations),
                 "Less robust than bracketed methods when the root is poorly conditioned.");
        snprintf(recommendation->alternatives,
                 sizeof(recommendation->alternatives),
                 "Bisection or Newton-Raphson when a derivative is available.");
        return NUMERIX_STATUS_OK;
    }

    recommendation->recommended_method = NUMERIX_METHOD_NONE;
    recommendation->confidence = 0;
    snprintf(recommendation->reason,
             sizeof(recommendation->reason),
             "Insufficient problem information for a reliable recommendation.");
    snprintf(recommendation->advantages,
             sizeof(recommendation->advantages),
             "none");
    snprintf(recommendation->limitations,
             sizeof(recommendation->limitations),
             "No valid bracket, initial guess, or derivative data was provided.");
    snprintf(recommendation->alternatives,
             sizeof(recommendation->alternatives),
             "Gather a bracket or a derivative before selecting a root solver.");
    return NUMERIX_STATUS_OK;
}
