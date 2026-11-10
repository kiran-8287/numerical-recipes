#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_fallback_history_init(NUMERIX_FallbackHistory *history)
{
    if (history == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(history, 0, sizeof(*history));
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_method_fallback(const NUMERIX_Problem *problem,
                                     NUMERIX_Method current_method,
                                     NUMERIX_MethodRecommendation *fallback,
                                     NUMERIX_FallbackHistory *history)
{
    if (problem == NULL || fallback == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_recommendation_init(fallback);
    if (history != NULL) {
        numerix_fallback_history_init(history);
    }

    if (current_method == NUMERIX_METHOD_NEWTON_RAPHSON) {
        if (problem->has_bracket) {
            fallback->recommended_method = NUMERIX_METHOD_BISECTION;
            snprintf(fallback->reason, sizeof(fallback->reason), "Newton-Raphson failed, so a bracketed bisection fallback is safer.");
            if (history != NULL) {
                history->records[history->count].attempted_method = current_method;
                history->records[history->count].fallback_method = fallback->recommended_method;
                snprintf(history->records[history->count].reason, sizeof(history->records[history->count].reason), "%s", fallback->reason);
                history->count = 1U;
            }
            return NUMERIX_STATUS_OK;
        }
        if (problem->has_initial_guess) {
            fallback->recommended_method = NUMERIX_METHOD_SECANT;
            snprintf(fallback->reason, sizeof(fallback->reason), "Derivative-based method failed; secant fallback remains available.");
            if (history != NULL) {
                history->records[history->count].attempted_method = current_method;
                history->records[history->count].fallback_method = fallback->recommended_method;
                snprintf(history->records[history->count].reason, sizeof(history->records[history->count].reason), "%s", fallback->reason);
                history->count = 1U;
            }
            return NUMERIX_STATUS_OK;
        }
    }

    fallback->recommended_method = NUMERIX_METHOD_NONE;
    snprintf(fallback->reason, sizeof(fallback->reason), "No valid fallback method could be recommended.");
    if (history != NULL) {
        history->records[history->count].attempted_method = current_method;
        history->records[history->count].fallback_method = NUMERIX_METHOD_NONE;
        snprintf(history->records[history->count].reason, sizeof(history->records[history->count].reason), "%s", fallback->reason);
        history->count = 1U;
    }
    return NUMERIX_STATUS_OK;
}
