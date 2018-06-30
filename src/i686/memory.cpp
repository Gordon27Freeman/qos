#include <defs.h>

#define SSE_MMREG_SIZE 16
#define MIN_LEN 0x40

static void small_memcpy(void *to, const void *from, size_t n)
{
	register uintptr_t dummy;
	asm volatile("rep; movsb" : "=&D"(to), "=&S"(from), "=&c"(dummy) : "0" (to), "1" (from),"2" (n) : "memory");
}

static void *linux_kernel_memcpy_impl(void *to, const void *from, size_t n)
{
	int d0, d1, d2;

	if(n < 4) {
		small_memcpy(to,from,n);
	}
	else
	{
		asm volatile(
			"rep ; movsl\n\t"
			"testb $2,%b4\n\t"
			"je 1f\n\t"
			"movsw\n"
			"1:\ttestb $1,%b4\n\t"
			"je 2f\n\t"
			"movsb\n"
			"2:"
			: "=&c" (d0), "=&D" (d1), "=&S" (d2)
			:"0" (n/4), "q" (n),"1" ((uintptr_t) to),"2" ((uintptr_t) from)
			: "memory"
		);
	}

	return (to);
}

void *memset(void *dest, int val, size_t count)
{
	asm("rep stosl;" : : "a"(val | (val << 8) | (val << 16) | (val << 24)), "D" ((size_t)dest), "c" (count / 4));
	asm("rep stosb;" : : "a"(val), "D" (((size_t)dest) + ((count / 4) * 4)), "c" (count - ((count / 4) * 4)));
	return dest;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	void *retval;
	size_t i;
	retval = dest;

	asm volatile(
		"	 prefetchnta (%0)\n"
		"	 prefetchnta 32(%0)\n"
		"	 prefetchnta 64(%0)\n"
		"	 prefetchnta 96(%0)\n"
		"	 prefetchnta 128(%0)\n"
		"	 prefetchnta 160(%0)\n"
		"	 prefetchnta 192(%0)\n"
		"	 prefetchnta 224(%0)\n"
		"	 prefetchnta 256(%0)\n"
		"	 prefetchnta 288(%0)\n"
		: : "r" (src)
	);

	if(count >= MIN_LEN)
	{
		register uintptr_t delta;
		delta = ((uintptr_t)dest)&(SSE_MMREG_SIZE-1);
		
		if(delta)
		{
			delta=SSE_MMREG_SIZE-delta;
			count -= delta;
			small_memcpy(dest, src, delta);
		}
		
		i = count >> 6;
		count&=63;
		
		if(((uintptr_t)src) & 15)
		{
			for(; i>0; i--)
			{
				asm volatile(
					"prefetchnta 320(%0)\n"
					"prefetchnta 352(%0)\n"
					"movups (%0), %%xmm0\n"
					"movups 16(%0), %%xmm1\n"
					"movups 32(%0), %%xmm2\n"
					"movups 48(%0), %%xmm3\n"
					"movntps %%xmm0, (%1)\n"
					"movntps %%xmm1, 16(%1)\n"
					"movntps %%xmm2, 32(%1)\n"
					"movntps %%xmm3, 48(%1)\n"
					:: "r" (src), "r" (dest) : "memory"
				);
				src = ((const unsigned char *)src) + 64;
				dest = ((unsigned char *)dest) + 64;
			}
		}
		else
		{
			for(; i>0; i--)
			{
				asm volatile(
					"prefetchnta 320(%0)\n"
					"prefetchnta 352(%0)\n"
					"movaps (%0), %%xmm0\n"
					"movaps 16(%0), %%xmm1\n"
					"movaps 32(%0), %%xmm2\n"
					"movaps 48(%0), %%xmm3\n"
					"movntps %%xmm0, (%1)\n"
					"movntps %%xmm1, 16(%1)\n"
					"movntps %%xmm2, 32(%1)\n"
					"movntps %%xmm3, 48(%1)\n"
					:: "r" (src), "r" (dest) : "memory"
				);
				src = ((const unsigned char *)src) + 64;
				dest = ((unsigned char *)dest) + 64;
			}
		}
		asm volatile("sfence":::"memory");
	}

	if(count) linux_kernel_memcpy_impl(dest, src, count);
	return retval;
}
