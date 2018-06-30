#include <i686/gdt.h>
using namespace GDT;

struct Entry
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

struct Pointer
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

static struct Entry gdt[3];
struct Pointer gdtp;

extern "C" void GDT_Flush();

static void SetGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
	gdt[num].base_low = base & 0xFFFF;
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;
	gdt[num].limit_low = limit & 0xFFFF;
	gdt[num].granularity = (limit >> 16) & 0xFF;
	gdt[num].granularity |= gran & 0xF0;
	gdt[num].access = access;
}

void GDT::Init()
{
	gdtp.limit = (sizeof(struct Entry) * 3) - 1;
	gdtp.base = (int)&gdt;

	SetGate(0, 0, 0, 0, 0);
	SetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	SetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	GDT_Flush();
}
