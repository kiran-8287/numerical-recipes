#include <math.h>
#include <stdio.h>
#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_convergence_monitor_init(NUMERIX_ConvergenceMonitor *monitor)
{
    if (monitor == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(monitor, 0, sizeof(*monitor));
    monitor->state = NUMERIX_CONVERGENCE_UNKNOWN;
    monitor->residual = NAN;
    monitor->previous_residual = NAN;
    monitor->current_value = NAN;
    monitor->previous_value = NAN;
    monitor->iterations = 0U;
    snprintf(monitor->summary, sizeof(monitor->summary), "not started");
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_convergence_monitor_update(NUMERIX_ConvergenceMonitor *monitor,
                                                double residual,
                                                double previous_residual,
                                                double current_value,
                                                double previous_value)
{
    if (monitor == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    monitor->previous_residual = previous_residual;
    monitor->residual = residual;
    monitor->previous_value = previous_value;
    monitor->current_value = current_value;
    monitor->iterations += 1U;

    if (residual <= 1e-12) {
        monitor->state = NUMERIX_CONVERGENCE_REACHED;
        snprintf(monitor->summary, sizeof(monitor->summary), "residual below tolerance");
        return NUMERIX_STATUS_OK;
    }

    if (previous_residual > 0.0 && residual > previous_residual * 1.5) {
        monitor->state = NUMERIX_CONVERGENCE_DIVERGING;
        snprintf(monitor->summary, sizeof(monitor->summary), "residual increasing");
        return NUMERIX_STATUS_OK;
    }

    if (previous_residual > 0.0 && fabs(residual - previous_residual) <= 1e-12) {
        monitor->state = NUMERIX_CONVERGENCE_STAGNANT;
        snprintf(monitor->summary, sizeof(monitor->summary), "residual stagnating");
        return NUMERIX_STATUS_OK;
    }

    if (previous_value != 0.0 && fabs(current_value - previous_value) > fabs(previous_value) * 1.5) {
        monitor->state = NUMERIX_CONVERGENCE_OSCILLATORY;
        snprintf(monitor->summary, sizeof(monitor->summary), "value oscillating");
        return NUMERIX_STATUS_OK;
    }

    monitor->state = NUMERIX_CONVERGENCE_PROGRESS;
    snprintf(monitor->summary, sizeof(monitor->summary), "progressing toward convergence");
    return NUMERIX_STATUS_OK;
}
