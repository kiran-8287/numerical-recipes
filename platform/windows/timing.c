#include "timing.h"

#if defined(_WIN32)
#include <windows.h>

double numerix_platform_monotonic_seconds(void)
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;

    if (QueryPerformanceFrequency(&frequency) == 0) {
        return 0.0;
    }

    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}
#else
#error "This file is only for Windows builds."
#endif
