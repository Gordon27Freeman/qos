#ifndef MEMORY_H
#define MEMORY_H

#include <defs.h>

void *memset(void *dest, int val, size_t count);
void *memcpy(void *dest, const void *src, size_t count);

#endif
