#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_compare_standard_libraries(const NUMERIX_Problem *problem,
                                                const NUMERIX_Method *methods,
                                                size_t method_count,
                                                NUMERIX_ExternalComparison *comparison)
{
    static const NUMERIX_ExternalLibrary libraries[] = {
        NUMERIX_LIBRARY_INTERNAL,
        NUMERIX_LIBRARY_GSL,
        NUMERIX_LIBRARY_BLAS,
        NUMERIX_LIBRARY_LAPACK,
        NUMERIX_LIBRARY_NUMERICAL_RECIPES
    };
    size_t i;

    if (problem == NULL || methods == NULL || comparison == NULL ||
        method_count == 0U || problem->function.evaluate == NULL ||
        (problem->has_reference_solution && !isfinite(problem->reference_solution))) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(comparison, 0, sizeof(*comparison));
    comparison->best_accuracy = NAN;
    comparison->has_reference = 0;

    for (i = 0U; i < sizeof(libraries) / sizeof(libraries[0]); ++i) {
        NUMERIX_ReferenceResult *entry = &comparison->entries[i];
        entry->library = libraries[i];
        entry->method = methods[0];
        entry->accuracy = NAN;
        entry->residual = NAN;
        entry->runtime_seconds = NAN;
        entry->available = 0;

        if (libraries[i] == NUMERIX_LIBRARY_INTERNAL) {
            NUMERIX_ComparisonResult local;
            NUMERIX_Status status = numerix_compare_methods(problem, methods,
                                                            method_count, &local);
            if (status == NUMERIX_STATUS_OK && local.has_winner) {
                size_t j;
                const NUMERIX_ComparisonEntry *winner = NULL;
                for (j = 0U; j < local.count; ++j) {
                    if (local.entries[j].converged &&
                        local.entries[j].method == local.winner_method &&
                        local.entries[j].value == local.winner_value) {
                        winner = &local.entries[j];
                        break;
                    }
                }
                if (winner != NULL) {
                    entry->method = winner->method;
                    entry->residual = winner->residual;
                    entry->runtime_seconds = winner->execution_time_seconds;
                    entry->available = 1;
                    if (problem->has_reference_solution) {
                        entry->accuracy = fabs(winner->value - problem->reference_solution);
                        comparison->best_accuracy = entry->accuracy;
                        comparison->has_reference = 1;
                    }
                    snprintf(entry->note, sizeof(entry->note),
                             "internal NUMERIX result; %s",
                             problem->has_reference_solution ?
                             "accuracy uses supplied exact solution" :
                             "no exact solution supplied; residual only");
                }
            } else {
                snprintf(entry->note, sizeof(entry->note),
                         "internal NUMERIX methods produced no verified result");
            }
        } else if (libraries[i] == NUMERIX_LIBRARY_NUMERICAL_RECIPES) {
            snprintf(entry->note, sizeof(entry->note),
                     "legacy routines are part of this archive; no separate baseline is configured");
        } else {
            snprintf(entry->note, sizeof(entry->note),
                     "external adapter is not implemented or configured");
        }
    }

    comparison->count = sizeof(libraries) / sizeof(libraries[0]);
    if (comparison->has_reference) {
        snprintf(comparison->summary, sizeof(comparison->summary),
                 "internal result plus %lu unavailable external baselines; absolute error %.3e",
                 (unsigned long)(comparison->count - 1U), comparison->best_accuracy);
    } else {
        snprintf(comparison->summary, sizeof(comparison->summary),
                 "internal residual baseline only; exact solution and external adapters unavailable");
    }
    return NUMERIX_STATUS_OK;
}
