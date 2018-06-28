#include <multiboot.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>

extern "C" void kmain(multiboot_header *multiboot, unsigned int magic)
{
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) asm volatile("cli; hlt;");

	GDT::Init();
	IDT::Init();
	ISR::Init();
	IRQ::Init();

	while (1) asm volatile("hlt");
}
