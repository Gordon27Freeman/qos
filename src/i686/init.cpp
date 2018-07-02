#include <i686/multiboot.h>
#include <i686/keyboard.h>
#include <i686/memory.h>
#include <i686/mouse.h>
#include <i686/timer.h>
#include <i686/gdt.h>
#include <i686/idt.h>
#include <i686/isr.h>
#include <i686/irq.h>

#include <graphics.h>
#include <mm.h>

extern "C" void init(multiboot_header *multiboot, unsigned int magic)
{
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC) asm volatile("cli; hlt;");
	GDT::Init();
	IDT::Init();
	ISR::Init();
	IRQ::Init();
	Timer::Init();
	Mouse::Init();
	Keyboard::Init();
	Memory::Init(multiboot->mem_upper + multiboot->mem_lower);
	Graphics::Init(multiboot->framebuffer_addr);
}
