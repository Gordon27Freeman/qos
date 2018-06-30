#include <i686/irq.h>
#include <i686/idt.h>
#include <i686/pio.h>
using namespace IRQ;

extern "C" void irq0();
extern "C" void irq1();
extern "C" void irq2();
extern "C" void irq3();
extern "C" void irq4();
extern "C" void irq5();
extern "C" void irq6();
extern "C" void irq7();
extern "C" void irq8();
extern "C" void irq9();
extern "C" void irq10();
extern "C" void irq11();
extern "C" void irq12();
extern "C" void irq13();
extern "C" void irq14();
extern "C" void irq15();

static void *irqRoutines[16] =
{
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static void Remap(void)
{
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
}

void IRQ::InstallHandler(int irq, void (*handler)(struct regs *r))
{
	irqRoutines[irq] = (void*)handler;
}

void IRQ::UninstallHandler(int irq)
{
	irqRoutines[irq] = 0;
}

void IRQ::Init()
{
	Remap();

	IDT::SetGate(32, (unsigned)irq0, 0x08, 0x8E);
	IDT::SetGate(33, (unsigned)irq1, 0x08, 0x8E);
	IDT::SetGate(34, (unsigned)irq2, 0x08, 0x8E);
	IDT::SetGate(35, (unsigned)irq3, 0x08, 0x8E);
	IDT::SetGate(36, (unsigned)irq4, 0x08, 0x8E);
	IDT::SetGate(37, (unsigned)irq5, 0x08, 0x8E);
	IDT::SetGate(38, (unsigned)irq6, 0x08, 0x8E);
	IDT::SetGate(39, (unsigned)irq7, 0x08, 0x8E);
	IDT::SetGate(40, (unsigned)irq8, 0x08, 0x8E);
	IDT::SetGate(41, (unsigned)irq9, 0x08, 0x8E);
	IDT::SetGate(42, (unsigned)irq10, 0x08, 0x8E);
	IDT::SetGate(43, (unsigned)irq11, 0x08, 0x8E);
	IDT::SetGate(44, (unsigned)irq12, 0x08, 0x8E);
	IDT::SetGate(45, (unsigned)irq13, 0x08, 0x8E);
	IDT::SetGate(46, (unsigned)irq14, 0x08, 0x8E);
	IDT::SetGate(47, (unsigned)irq15, 0x08, 0x8E);
}

extern "C" void IRQHandler(struct regs *r)
{
	void (*handler)(struct regs *r);

	handler = (void (*)(regs*))irqRoutines[r->int_no - 32];
	if (handler) handler(r);

	if (r->int_no >= 40) outb(0xA0, 0x20);
	outb(0x20, 0x20);
}
