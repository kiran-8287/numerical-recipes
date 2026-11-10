#include <stdio.h>
#include "numerix.h"

int main(void)
{
    NUMERIX_StepExecutionLog log;
    NUMERIX_StepRecord record;

    if (numerix_step_execution_log_init(&log) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "step log init failed\n");
        return 1;
    }

    record.iteration = 1U;
    record.x = 0.0;
    record.value = -4.0;
    record.next_x = 2.0;
    record.message[0] = '\0';
    snprintf(record.message, sizeof(record.message), "midpoint update");

    if (numerix_step_execution_log_append(&log, &record) != NUMERIX_STATUS_OK) {
        fprintf(stderr, "step append failed\n");
        return 2;
    }

    if (log.count != 1U) {
        fprintf(stderr, "step count wrong\n");
        return 3;
    }

    printf("Phase 12 step execution OK\n");
    return 0;
}
