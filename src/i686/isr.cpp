#include <i686/isr.h>
#include <i686/idt.h>
#include <i686/regs.h>

#include <graphics.h>
using namespace ISR;

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

void ISR::Init()
{
	IDT::SetGate(0, (unsigned)isr0, 0x08, 0x8E);
	IDT::SetGate(1, (unsigned)isr1, 0x08, 0x8E);
	IDT::SetGate(2, (unsigned)isr2, 0x08, 0x8E);
	IDT::SetGate(3, (unsigned)isr3, 0x08, 0x8E);
	IDT::SetGate(4, (unsigned)isr4, 0x08, 0x8E);
	IDT::SetGate(5, (unsigned)isr5, 0x08, 0x8E);
	IDT::SetGate(6, (unsigned)isr6, 0x08, 0x8E);
	IDT::SetGate(7, (unsigned)isr7, 0x08, 0x8E);
	IDT::SetGate(8, (unsigned)isr8, 0x08, 0x8E);
	IDT::SetGate(9, (unsigned)isr9, 0x08, 0x8E);
	IDT::SetGate(10, (unsigned)isr10, 0x08, 0x8E);
	IDT::SetGate(11, (unsigned)isr11, 0x08, 0x8E);
	IDT::SetGate(12, (unsigned)isr12, 0x08, 0x8E);
	IDT::SetGate(13, (unsigned)isr13, 0x08, 0x8E);
	IDT::SetGate(14, (unsigned)isr14, 0x08, 0x8E);
	IDT::SetGate(15, (unsigned)isr15, 0x08, 0x8E);
	IDT::SetGate(16, (unsigned)isr16, 0x08, 0x8E);
	IDT::SetGate(17, (unsigned)isr17, 0x08, 0x8E);
	IDT::SetGate(18, (unsigned)isr18, 0x08, 0x8E);
	IDT::SetGate(19, (unsigned)isr19, 0x08, 0x8E);
	IDT::SetGate(20, (unsigned)isr20, 0x08, 0x8E);
	IDT::SetGate(21, (unsigned)isr21, 0x08, 0x8E);
	IDT::SetGate(22, (unsigned)isr22, 0x08, 0x8E);
	IDT::SetGate(23, (unsigned)isr23, 0x08, 0x8E);
	IDT::SetGate(24, (unsigned)isr24, 0x08, 0x8E);
	IDT::SetGate(25, (unsigned)isr25, 0x08, 0x8E);
	IDT::SetGate(26, (unsigned)isr26, 0x08, 0x8E);
	IDT::SetGate(27, (unsigned)isr27, 0x08, 0x8E);
	IDT::SetGate(28, (unsigned)isr28, 0x08, 0x8E);
	IDT::SetGate(29, (unsigned)isr29, 0x08, 0x8E);
	IDT::SetGate(30, (unsigned)isr30, 0x08, 0x8E);
	IDT::SetGate(31, (unsigned)isr31, 0x08, 0x8E);
}

static const char *exceptionMessages[] =
{
	"Division By Zero",
	"Debug",
	"Non Maskable Interrupt",
	"Breakpoint",
	"Into Detected Overflow",
	"Out of Bounds",
	"Invalid Opcode",
	"No Coprocessor",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt",
	"Coprocessor Fault",
	"Alignment Check",
	"Machine Check",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved",
	"Reserved"
};

static unsigned int buffer[1000 * 1000];
static unsigned int *framebuffer = buffer + 10000;

extern "C" void FaultHandler(struct regs *r)
{
	if (r->int_no < 32)
	{
		Graphics::FillRect(framebuffer, 800, 0, 0, 800, 600, 0x303030);
		Graphics::DrawString(framebuffer, 800, exceptionMessages[r->int_no], 16, 10, 0xffffff);
		Graphics::DrawString(framebuffer, 800, "Exception Occured. System Halted!", 16, 28, 0xffffff);
		Graphics::DrawFullscreenBuffer(framebuffer);
		while (1) asm volatile("cli; hlt;");
	}
}
