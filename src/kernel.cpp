#include <multiboot.h>
#include <gdt.h>
#include <idt.h>

extern "C" void kmain(multiboot_header *multiboot, unsigned int magic)
{
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) asm volatile("cli; hlt;");

	GDT::Init();
	IDT::Init();

	while (1) asm volatile("hlt");
}
