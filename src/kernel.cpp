#include <multiboot.h>
#include <graphics.h>
#include <memory.h>
#include <mouse.h>
#include <timer.h>
#include <gdt.h>
#include <idt.h>
#include <isr.h>
#include <irq.h>

unsigned int window[160 * 120];

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

	// -------
	Graphics::FillRect(window, 160, 0, 0, 160, 120, 0xb0a090);

	Graphics::HorizontalLine(window, 160, 0, 119, 160, 0x907060);
	Graphics::VerticalLine(window, 160, 159, 0, 120, 0x907060);
	Graphics::HorizontalLine(window, 160, 0, 0, 160, 0xf0c0b0);
	Graphics::VerticalLine(window, 160, 0, 0, 120, 0xf0c0b0);
	// -------

	Graphics::FillRect(window, 160, 2, 2, 156, 19, 0x201080);

	// -------
	Graphics::FillRect(window, 160, 141, 4, 15, 15, 0xb0a090);

	Graphics::HorizontalLine(window, 160, 141, 18, 15, 0x907060);
	Graphics::VerticalLine(window, 160, 155, 4, 15, 0x907060);
	Graphics::HorizontalLine(window, 160, 141, 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(window, 160, 141, 4, 15, 0xf0c0b0);

	Graphics::DrawChar(window, 160, 'x', 145, 3, 0x101010);
	// -------

	Graphics::DrawString(window, 160, "Hello, World!", 6, 4, 0xe0e0e0);
	Graphics::DrawString(window, 160, "Simple window", 6, 25, 0x101010);

	Graphics::DrawBuffer(window, 100, 100, 160, 120);

	while (1) asm volatile("hlt");
}
