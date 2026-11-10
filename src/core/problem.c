#include <stddef.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_function_init(NUMERIX_Function *function,
                                     NUMERIX_EvalCallback evaluate,
                                     NUMERIX_DerivativeCallback derivative,
                                     void *user_data)
{
    if (function == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(function, 0, sizeof(*function));
    function->type = NUMERIX_FUNCTION_CALLBACK;
    function->evaluate = evaluate;
    function->derivative = derivative;
    function->user_data = user_data;
    function->domain_known = 0;
    function->domain_min = 0.0;
    function->domain_max = 0.0;
    function->continuous = 0;
    function->differentiable = 0;
    function->owns_resources = 0;

    if (evaluate == NULL) {
        function->type = NUMERIX_FUNCTION_NONE;
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_problem_init(NUMERIX_Problem *problem,
                                   const NUMERIX_Function *function)
{
    if (problem == NULL || function == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(problem, 0, sizeof(*problem));
    problem->function = *function;
    problem->x0 = 0.0;
    problem->x1 = 0.0;
    problem->target = 0.0;
    problem->has_bracket = 0;
    problem->has_initial_guess = 0;
    problem->preferred_method = NUMERIX_METHOD_NONE;
    numerix_options_init(&problem->options);

    if (function->evaluate == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    return NUMERIX_STATUS_OK;
}
