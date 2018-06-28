#include <defs.h>

void memset(void *dest, int data, size_t count)
{
	size_t n = 0;
	unsigned char *ptr = (unsigned char *)dest;
	while(n < count)
	{
		ptr[n] = (unsigned char)data;
		n++;
	}
}
