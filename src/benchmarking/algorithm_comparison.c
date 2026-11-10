#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"
#include "timing.h"

NUMERIX_Status numerix_comparison_result_init(NUMERIX_ComparisonResult *result)
{
    if (result == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(result, 0, sizeof(*result));
    result->winner_method = NUMERIX_METHOD_NONE;
    result->winner_value = NAN;
    result->winner_residual = NAN;
    result->has_winner = 0;
    return NUMERIX_STATUS_OK;
}

typedef struct {
    const NUMERIX_Function *source;
    size_t function_evaluations;
    int counter_overflow;
} NUMERIX_EvaluationCounter;

static double numerix_counted_evaluate(double x, void *user_data)
{
    NUMERIX_EvaluationCounter *counter = (NUMERIX_EvaluationCounter *)user_data;
    if (counter->function_evaluations < (size_t)-1) {
        counter->function_evaluations++;
    } else {
        counter->counter_overflow = 1;
    }
    return numerix_function_evaluate(counter->source, x, NULL);
}

static double numerix_counted_derivative(double x, void *user_data)
{
    NUMERIX_EvaluationCounter *counter = (NUMERIX_EvaluationCounter *)user_data;
    if (counter->source->derivative == NULL) {
        return NAN;
    }
    return counter->source->derivative(x, counter->source->user_data);
}

static NUMERIX_Status numerix_compare_single_method(const NUMERIX_Problem *problem,
                                                   NUMERIX_Method method,
                                                   NUMERIX_ComparisonEntry *entry)
{
    NUMERIX_Problem measured_problem;
    NUMERIX_Function measured_function;
    NUMERIX_EvaluationCounter counter;
    double value = NAN;
    double residual = NAN;
    double tolerance = 1e-8;
    double secant_x1;
    double start_time;
    double end_time;
    int supported = 0;

    if (problem == NULL || entry == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }
    secant_x1 = problem->x0;

    if (problem->options.absolute_tolerance > 0.0) {
        tolerance = problem->options.absolute_tolerance;
    }

    memset(entry, 0, sizeof(*entry));
    entry->method = method;
    entry->status = NUMERIX_STATUS_NOT_SUPPORTED;
    entry->converged = 0;
    entry->iterations = 0U;
    entry->function_evaluations = 0U;
    entry->execution_time_seconds = 0.0;
    entry->estimated_error = NAN;
    entry->value = NAN;
    entry->residual = NAN;
    snprintf(entry->summary, sizeof(entry->summary), "not applicable");

    if (problem->function.evaluate == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    if (method == NUMERIX_METHOD_BISECTION && problem->has_bracket) {
        supported = 1;
    } else if (method == NUMERIX_METHOD_SECANT) {
        if (problem->has_bracket) {
            secant_x1 = problem->x1;
        } else if (problem->has_initial_guess) {
            secant_x1 = problem->x0 + 1.0;
        } else {
            return NUMERIX_STATUS_NOT_SUPPORTED;
        }
        supported = 1;
    } else if (method == NUMERIX_METHOD_NEWTON_RAPHSON &&
               problem->has_initial_guess && problem->function.derivative != NULL) {
        supported = 1;
    }

    if (!supported) {
        return NUMERIX_STATUS_NOT_SUPPORTED;
    }

    counter.source = &problem->function;
    counter.function_evaluations = 0U;
    counter.counter_overflow = 0;
    numerix_function_init(&measured_function, numerix_counted_evaluate,
                          problem->function.derivative != NULL ?
                              numerix_counted_derivative : NULL,
                          &counter);
    measured_problem = *problem;
    measured_problem.function = measured_function;

    start_time = numerix_platform_monotonic_seconds();
    if (method == NUMERIX_METHOD_BISECTION) {
        value = numerix_bisection(&measured_problem.function,
                                  problem->x0, problem->x1, tolerance);
    } else if (method == NUMERIX_METHOD_SECANT) {
        value = numerix_secant(&measured_problem.function,
                               problem->x0, secant_x1, tolerance);
    } else if (method == NUMERIX_METHOD_NEWTON_RAPHSON) {
        value = numerix_newton_raphson(&measured_problem.function,
                                       problem->x0, tolerance);
    }
    if (isfinite(value)) {
        double function_value = numerix_function_evaluate(&measured_problem.function,
                                                          value, NULL);
        residual = isfinite(function_value) ? fabs(function_value) : NAN;
    }
    end_time = numerix_platform_monotonic_seconds();

    entry->value = value;
    entry->residual = residual;
    entry->estimated_error = NAN;
    entry->converged = isfinite(value) && isfinite(residual) &&
                       residual <= (tolerance * 10.0);
    entry->status = entry->converged ? NUMERIX_STATUS_OK :
                                       NUMERIX_STATUS_CONVERGENCE_FAILURE;
    entry->function_evaluations = counter.function_evaluations;
    entry->iterations = 0U;
    entry->execution_time_seconds = isfinite(start_time) && isfinite(end_time) &&
                                    end_time >= start_time && end_time > 0.0 ?
                                    end_time - start_time : NAN;
    snprintf(entry->summary, sizeof(entry->summary),
             "iterations unavailable; residual is measured; %s",
             counter.counter_overflow ? "evaluation counter saturated" :
                                       "function evaluations counted");
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_compare_methods(const NUMERIX_Problem *problem,
                                      const NUMERIX_Method *methods,
                                      size_t method_count,
                                      NUMERIX_ComparisonResult *result)
{
    size_t i;
    size_t best_index = 0U;
    int found_candidate = 0;

    if (problem == NULL || methods == NULL || result == NULL || method_count == 0U) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    numerix_comparison_result_init(result);

    for (i = 0U; i < method_count && result->count < 8U; ++i) {
        NUMERIX_ComparisonEntry *entry = &result->entries[result->count];
        NUMERIX_Status status = numerix_compare_single_method(problem, methods[i], entry);

        if (status == NUMERIX_STATUS_OK || status == NUMERIX_STATUS_NOT_SUPPORTED) {
            result->count++;
            if (entry->converged) {
                if (!found_candidate || entry->residual < result->entries[best_index].residual) {
                    best_index = result->count - 1U;
                    found_candidate = 1;
                }
            }
        }
    }

    if (result->count == 0U) {
        snprintf(result->summary, sizeof(result->summary), "no comparable methods were applicable");
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    if (!found_candidate) {
        result->winner_method = NUMERIX_METHOD_NONE;
        result->winner_value = NAN;
        result->winner_residual = NAN;
        result->has_winner = 0;
        snprintf(result->summary, sizeof(result->summary), "no converged candidate available");
        return NUMERIX_STATUS_CONVERGENCE_FAILURE;
    }

    result->winner_method = result->entries[best_index].method;
    result->winner_value = result->entries[best_index].value;
    result->winner_residual = result->entries[best_index].residual;
    result->has_winner = 1;
    snprintf(result->summary,
             sizeof(result->summary),
             "winner=%d value=%.12g residual=%.3e",
             result->winner_method,
             result->winner_value,
             result->winner_residual);

    return NUMERIX_STATUS_OK;
}
