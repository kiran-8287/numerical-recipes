#include <stdlib.h>
#include "numerix.h"

void *numerix_malloc(size_t bytes)
{
    if (bytes == 0U) {
        bytes = 1U;
    }

    return malloc(bytes);
}

void numerix_free(void *ptr)
{
    free(ptr);
}
