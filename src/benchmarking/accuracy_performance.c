#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_accuracy_study(const NUMERIX_Problem *problem,
                                     const NUMERIX_Method *methods,
                                     size_t method_count,
                                     const double *tolerances,
                                     size_t tolerance_count,
                                     NUMERIX_AccuracyStudy *study)
{
    size_t i;
    size_t sample_count = 0U;
    double best_residual = INFINITY;
    double best_error = INFINITY;
    double best_error_tolerance = NAN;

    if (problem == NULL || methods == NULL || tolerances == NULL || study == NULL ||
        method_count == 0U || tolerance_count == 0U ||
        problem->function.evaluate == NULL ||
        (problem->has_reference_solution && !isfinite(problem->reference_solution))) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(study, 0, sizeof(*study));
    study->best_error = NAN;
    study->best_tolerance = NAN;
    study->best_residual = NAN;

    for (i = 0U; i < tolerance_count && sample_count < 16U; ++i) {
        NUMERIX_Problem probe = *problem;
        NUMERIX_ComparisonResult comparison;
        const NUMERIX_ComparisonEntry *winner = NULL;
        size_t j;

        if (!isfinite(tolerances[i]) || tolerances[i] <= 0.0) {
            return NUMERIX_STATUS_INVALID_ARGUMENT;
        }
        probe.options.absolute_tolerance = tolerances[i];
        if (numerix_compare_methods(&probe, methods, method_count, &comparison) !=
            NUMERIX_STATUS_OK || !comparison.has_winner) {
            continue;
        }
        for (j = 0U; j < comparison.count; ++j) {
            if (comparison.entries[j].converged &&
                comparison.entries[j].method == comparison.winner_method &&
                comparison.entries[j].value == comparison.winner_value) {
                winner = &comparison.entries[j];
                break;
            }
        }
        if (winner == NULL) {
            continue;
        }

        study->samples[sample_count].tolerance = tolerances[i];
        study->samples[sample_count].method = winner->method;
        study->samples[sample_count].actual_error = problem->has_reference_solution ?
            fabs(winner->value - problem->reference_solution) : NAN;
        study->samples[sample_count].residual = winner->residual;
        study->samples[sample_count].execution_time_seconds = winner->execution_time_seconds;
        study->samples[sample_count].iterations = winner->iterations;
        study->samples[sample_count].function_evaluations = winner->function_evaluations;
        study->samples[sample_count].converged = winner->converged;
        snprintf(study->samples[sample_count].note,
                 sizeof(study->samples[sample_count].note),
                 "measured residual; iterations not instrumented");

        if (isfinite(winner->residual) && winner->residual < best_residual) {
            best_residual = winner->residual;
        }
        if (problem->has_reference_solution &&
            isfinite(study->samples[sample_count].actual_error) &&
            study->samples[sample_count].actual_error < best_error) {
            best_error = study->samples[sample_count].actual_error;
            best_error_tolerance = tolerances[i];
        }
        sample_count++;
    }

    study->count = sample_count;
    if (sample_count == 0U) {
        snprintf(study->summary, sizeof(study->summary),
                 "no successful accuracy samples");
        return NUMERIX_STATUS_CONVERGENCE_FAILURE;
    }

    study->best_residual = best_residual;
    if (problem->has_reference_solution) {
        study->best_error = best_error;
        study->best_tolerance = best_error_tolerance;
        snprintf(study->summary, sizeof(study->summary),
                 "%lu samples; best measured error %.3e and residual %.3e",
                 (unsigned long)sample_count, best_error, best_residual);
    } else {
        snprintf(study->summary, sizeof(study->summary),
                 "%lu samples; no exact reference supplied; best residual %.3e",
                 (unsigned long)sample_count, best_residual);
    }
    return NUMERIX_STATUS_OK;
}
