#include "timing.h"

#if !defined(_WIN32)
#include <time.h>

double numerix_platform_monotonic_seconds(void)
{
    struct timespec ts;

    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        return 0.0;
    }

    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
}
#else
#error "This file is only for non-Windows builds."
#endif
