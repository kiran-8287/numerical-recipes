#include <string.h>
#include "numerix.h"

NUMERIX_Status numerix_step_execution_log_init(NUMERIX_StepExecutionLog *log)
{
    if (log == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    memset(log, 0, sizeof(*log));
    log->enabled = 1;
    return NUMERIX_STATUS_OK;
}

NUMERIX_Status numerix_step_execution_log_append(NUMERIX_StepExecutionLog *log,
                                                const NUMERIX_StepRecord *record)
{
    if (log == NULL || record == NULL) {
        return NUMERIX_STATUS_INVALID_ARGUMENT;
    }

    if (log->count < 64U) {
        log->records[log->count] = *record;
        log->count++;
        return NUMERIX_STATUS_OK;
    }

    memmove(&log->records[0],
            &log->records[1],
            sizeof(log->records[0]) * (64U - 1U));
    log->records[63U] = *record;
    return NUMERIX_STATUS_OK;
}
