#include <stdio.h>
#include <string.h>
#include "numerix.h"

const char *numerix_error_code_string(NUMERIX_ErrorCode code)
{
    switch (code) {
        case NUMERIX_ERROR_NONE:
            return "NONE";
        case NUMERIX_ERROR_INVALID_INPUT:
            return "INVALID_INPUT";
        case NUMERIX_ERROR_INVALID_INTERVAL:
            return "INVALID_INTERVAL";
        case NUMERIX_ERROR_DIVISION_BY_ZERO:
            return "DIVISION_BY_ZERO";
        case NUMERIX_ERROR_DOMAIN_VIOLATION:
            return "DOMAIN_VIOLATION";
        case NUMERIX_ERROR_SINGULAR_MATRIX:
            return "SINGULAR_MATRIX";
        case NUMERIX_ERROR_NEARLY_SINGULAR_MATRIX:
            return "NEARLY_SINGULAR_MATRIX";
        case NUMERIX_ERROR_OVERFLOW:
            return "OVERFLOW";
        case NUMERIX_ERROR_UNDERFLOW:
            return "UNDERFLOW";
        case NUMERIX_ERROR_NAN_RESULT:
            return "NAN_RESULT";
        case NUMERIX_ERROR_INFINITY_RESULT:
            return "INFINITY_RESULT";
        case NUMERIX_ERROR_DIVERGENCE:
            return "DIVERGENCE";
        case NUMERIX_ERROR_NONCONVERGENCE:
            return "NONCONVERGENCE";
        case NUMERIX_ERROR_STAGNATION:
            return "STAGNATION";
        case NUMERIX_ERROR_OSCILLATION:
            return "OSCILLATION";
        case NUMERIX_ERROR_ILL_CONDITIONING:
            return "ILL_CONDITIONING";
        case NUMERIX_ERROR_NUMERICAL_INSTABILITY:
            return "NUMERICAL_INSTABILITY";
        case NUMERIX_ERROR_RESOURCE_LIMIT_REACHED:
            return "RESOURCE_LIMIT_REACHED";
        case NUMERIX_ERROR_CANCELLATION:
            return "CANCELLATION";
        case NUMERIX_ERROR_DERIVATIVE_ZERO:
            return "DERIVATIVE_ZERO";
        default:
            return "UNKNOWN_ERROR";
    }
}

NUMERIX_Status numerix_error_report_init(NUMERIX_ErrorReport *report)
{
    if (report == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(report, 0, sizeof(*report));
    report->code = NUMERIX_ERROR_NONE;
    report->method = NUMERIX_METHOD_NONE;
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_error_detect(NUMERIX_Method method,
                                   NUMERIX_ErrorCode code,
                                   const char *context,
                                   const char *details,
                                   NUMERIX_ErrorReport *report)
{
    if (report == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_error_report_init(report);
    report->code = code;
    report->method = method;

    if (context != NULL && context[0] != '\0') {
        snprintf(report->where, sizeof(report->where), "%s", context);
    } else {
        snprintf(report->where, sizeof(report->where), "numerical solver state");
    }

    switch (code) {
        case NUMERIX_ERROR_INVALID_INPUT:
            snprintf(report->what, sizeof(report->what), "Invalid input was supplied to the numerical routine.");
            snprintf(report->why, sizeof(report->why), "The problem definition is inconsistent or missing required values.");
            snprintf(report->action, sizeof(report->action), "Check the inputs and provide valid bounds, guess values, or function callbacks.");
            snprintf(report->alternative, sizeof(report->alternative), "Validate the problem definition before invoking a solver.");
            break;
        case NUMERIX_ERROR_INVALID_INTERVAL:
            snprintf(report->what, sizeof(report->what), "The interval was invalid for the selected method.");
            snprintf(report->why, sizeof(report->why), "The lower and upper bounds do not satisfy the method requirements or are reversed.");
            snprintf(report->action, sizeof(report->action), "Ensure the interval is ordered and consistent with the chosen method.");
            snprintf(report->alternative, sizeof(report->alternative), "Switch to a bracket-based method only when the interval is valid.");
            break;
        case NUMERIX_ERROR_DIVISION_BY_ZERO:
            snprintf(report->what, sizeof(report->what), "Division by zero was encountered during evaluation.");
            snprintf(report->why, sizeof(report->why), "A denominator approached zero, which makes the local model undefined.");
            snprintf(report->action, sizeof(report->action), "Rescale the problem or avoid singular points and retry with a different starting state.");
            snprintf(report->alternative, sizeof(report->alternative), "Use a method that avoids evaluating the singular point directly.");
            break;
        case NUMERIX_ERROR_DERIVATIVE_ZERO:
            snprintf(report->what, sizeof(report->what), "The derivative evaluated to approximately zero.");
            snprintf(report->why, sizeof(report->why), "The current iteration is at or near a stationary point.");
            snprintf(report->action, sizeof(report->action), "Try a different initial guess or use a bracket-based method such as bisection.");
            snprintf(report->alternative, sizeof(report->alternative), "Use a bracketed or secant method when the derivative is unreliable near zero.");
            break;
        case NUMERIX_ERROR_SINGULAR_MATRIX:
            snprintf(report->what, sizeof(report->what), "A singular matrix was encountered.");
            snprintf(report->why, sizeof(report->why), "The matrix is rank-deficient and cannot be inverted reliably.");
            snprintf(report->action, sizeof(report->action), "Regularize the system or reformulate the problem using a well-conditioned matrix.");
            snprintf(report->alternative, sizeof(report->alternative), "Use pivoting, LU decomposition, or a different formulation when possible.");
            break;
        case NUMERIX_ERROR_NEARLY_SINGULAR_MATRIX:
            snprintf(report->what, sizeof(report->what), "The matrix is nearly singular and numerically unstable.");
            snprintf(report->why, sizeof(report->why), "The condition number is large enough that small perturbations can dominate the solution.");
            snprintf(report->action, sizeof(report->action), "Improve scaling or regularization and check whether the system is ill-conditioned.");
            snprintf(report->alternative, sizeof(report->alternative), "Use a more stable formulation or method with better conditioning.");
            break;
        default:
            snprintf(report->what, sizeof(report->what), "An unexpected numerical problem occurred.");
            snprintf(report->why, sizeof(report->why), "The solver encountered a condition that could not be validated as safe.");
            snprintf(report->action, sizeof(report->action), "Review the numerical inputs and choose a better-conditioned problem setup.");
            snprintf(report->alternative, sizeof(report->alternative), "Verify the solver assumptions and compare against a more robust method.");
            break;
    }

    if (details != NULL && details[0] != '\0') {
        snprintf(report->what, sizeof(report->what), "%s %s", report->what, details);
    }

    return NUMERIX_STATUS_OK;
}
