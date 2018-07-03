#include <mm.h>
using namespace Memory;

#define FREE 0xfe
#define USED 0xa1
#define UNUSED 0x01

extern unsigned int kernel_end, kernel_start;

struct Allocation
{
	unsigned char type;
	unsigned int size;
	unsigned int *addr;
	unsigned int next;
	unsigned int prev;
};

static struct Allocation memoryMap[100000];
static unsigned int freeMemory;

void Memory::Init(unsigned int totalMemory)
{
	freeMemory = totalMemory * 1024 - ((int)&kernel_end - (int)&kernel_start);
	memoryMap[0].addr = &kernel_end;
	memoryMap[0].size = freeMemory;
	memoryMap[0].type = FREE;
	int i = 1;
	while (i < 100000)
	{
		memoryMap[i].type = UNUSED;
		i++;
	}
}

void *Memory::Alloc(size_t size)
{
	int n = 100000, f = 0;
	while (n >= 0)
	{
		if (memoryMap[n].type == FREE && memoryMap[n].size >= size)
		{
			while (f < 100000)
			{
				if (memoryMap[f].type == UNUSED) break;
				f++;
			}
			if (f == 99999) return 0;
			memoryMap[f].addr = memoryMap[n].addr;
			memoryMap[f].size = size;
			memoryMap[f].next = n;
			memoryMap[f].prev = memoryMap[n].prev;
			memoryMap[f].type = USED;
			memoryMap[n].addr += size;
			memoryMap[n].size -= size;
			memoryMap[n].prev = f;
			return (void *)memoryMap[f].addr;
		}
		n--;
	}
	return 0;
}

void Memory::Free(void *obj)
{
	int n = 0;
	while (n < 100000)
	{
		if ((int)memoryMap[n].addr == (int)obj)
		{
			memoryMap[n].type = FREE;
			if (memoryMap[memoryMap[n].next].type == FREE && memoryMap[n].next != 0)
			{
				memoryMap[memoryMap[n].next].type = UNUSED;
				memoryMap[n].size += memoryMap[memoryMap[n].next].size;
				memoryMap[n].next = memoryMap[memoryMap[n].next].next;
			}
			if (memoryMap[memoryMap[n].prev].type == FREE && memoryMap[n].prev != 0)
			{
				memoryMap[n].type = UNUSED;
				memoryMap[memoryMap[n].prev].size += memoryMap[n].size;
				memoryMap[memoryMap[n].prev].addr = memoryMap[n].addr;
				memoryMap[memoryMap[n].prev].next = memoryMap[n].next;
			}
			break;
		}
		n++;
	}
}

void *Memory::Realloc(void *obj, size_t size)
{
	Free(obj);
	return Alloc(size);
}
