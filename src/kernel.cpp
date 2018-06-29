#include <multiboot.h>
#include <graphics.h>
#include <memory.h>
#include <mouse.h>
#include <timer.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>

extern "C" void kmain(multiboot_header *multiboot, unsigned int magic)
{
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) asm volatile("cli; hlt;");

	Graphics::Init(multiboot->framebuffer_addr);
	GDT::Init();
	IDT::Init();
	ISR::Init();
	IRQ::Init();
	Timer::Init();
	Mouse::Init();

	while (1) asm volatile("hlt");
}
