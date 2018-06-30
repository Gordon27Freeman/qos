#ifndef MM_H
#define MM_H

#include <defs.h>

namespace Memory
{
	void Init(unsigned int totalMemory);
	void *Alloc(size_t size);
	void Free(void *obj);
	void *Realloc(void *obj, size_t size);
}

#endif
