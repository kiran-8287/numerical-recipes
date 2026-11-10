#include "numerix.h"

const char *numerix_status_string(NUMERIX_Status status)
{
    switch (status) {
        case NUMERIX_STATUS_OK:
            return "OK";
        case NUMERIX_STATUS_INVALID_ARGUMENT:
            return "INVALID_ARGUMENT";
        case NUMERIX_STATUS_ALLOCATION_FAILED:
            return "ALLOCATION_FAILED";
        case NUMERIX_STATUS_CONVERGENCE_FAILURE:
            return "CONVERGENCE_FAILURE";
        case NUMERIX_STATUS_NONFINITE_RESULT:
            return "NONFINITE_RESULT";
        case NUMERIX_STATUS_CANCELLED:
            return "CANCELLED";
        case NUMERIX_STATUS_NOT_SUPPORTED:
            return "NOT_SUPPORTED";
        case NUMERIX_STATUS_UNAVAILABLE:
            return "UNAVAILABLE";
        case NUMERIX_STATUS_WARNING:
            return "WARNING";
        case NUMERIX_STATUS_ERROR:
            return "ERROR";
        case NUMERIX_STATUS_SINGULAR_MATRIX:
            return "SINGULAR_MATRIX";
        case NUMERIX_STATUS_NUMERICAL_FAILURE:
            return "NUMERICAL_FAILURE";
        case NUMERIX_STATUS_RESOURCE_LIMIT:
            return "RESOURCE_LIMIT";
        default:
            return "UNKNOWN";
    }
}
