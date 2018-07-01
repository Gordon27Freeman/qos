#include <i686/timer.h>
#include <i686/regs.h>
#include <i686/irq.h>
#include <i686/pio.h>

#include <gui.h>
using namespace Timer;

static unsigned long long current_ticks;

static void Handler(struct regs *r)
{
	current_ticks++;
}

static void SetPhase(int hz)
{
    int divisor = 1193182 / hz;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, divisor >> 8);
}

void Timer::Wait(int ticks)
{
	unsigned long long eticks = current_ticks + ticks;
	while (current_ticks < eticks) asm volatile("hlt");
}

void Timer::Init()
{
	IRQ::InstallHandler(0, Handler);
	SetPhase(1000);
	asm volatile("sti");
}
