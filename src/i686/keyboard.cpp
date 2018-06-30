#include <keyboard.h>
#include <regs.h>
#include <pio.h>
#include <irq.h>
using namespace Keyboard;

static void Handler(struct regs *r)
{
	unsigned char scancode;
	scancode = inb(0x60);
}

void Keyboard::Init()
{
	IRQ::InstallHandler(1, Handler);
}
