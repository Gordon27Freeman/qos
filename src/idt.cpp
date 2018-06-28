#include <idt.h>
#include <memory.h>
using namespace IDT;

struct Entry
{
	unsigned short base_low;
	unsigned short selector;
	unsigned char zero;
	unsigned char flags;
	unsigned short base_high;
} __attribute__((packed));

struct Pointer
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

static struct Entry idt[256];
struct Pointer idtp;

extern "C" void IDT_Load();

static void SetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
	idt[num].base_low = base & 0xFFFF;
	idt[num].base_high = (base >> 16) & 0xFFFF;
	idt[num].selector = sel;
	idt[num].zero = 0;
	idt[num].flags = flags;
}

void IDT::Init()
{
	idtp.limit = (sizeof(struct Entry) * 256) - 1;
	idtp.base = (int)&idt;

	memset(&idt, 0, sizeof(struct Entry) * 256);

	IDT_Load();
}