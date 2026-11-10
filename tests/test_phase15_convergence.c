#include <stdio.h>
#include "numerix.h"

int main(void)
{
    NUMERIX_ConvergenceMonitor monitor;

    if (numerix_convergence_monitor_init(&monitor) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "monitor init failed\n");
        return 1;
    }

    if (numerix_convergence_monitor_update(&monitor, 1.0, 2.0, 0.0, 1.0) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "monitor update failed\n");
        return 2;
    }

    if (monitor.state == NUMERIX_CONVERGENCE_UNKNOWN) {
        fprintf(stderr, "monitor state invalid\n");
        return 3;
    }

    printf("Phase 15 convergence monitoring OK\n");
    return 0;
}
