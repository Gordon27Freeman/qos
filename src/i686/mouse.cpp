#include <i686/mouse.h>
#include <i686/regs.h>
#include <i686/irq.h>
#include <i686/pio.h>

using namespace Mouse;

static int MouseX = 400, MouseY = 300, MousePrevX = MouseX, MousePrevY = MouseY;
static char Cycle = 0, Byte[3];

static char LeftButton = 0, RightButton = 0;

static void Handler(struct regs *r)
{
	char AccX = 0, AccY = 0;

	switch (Cycle)
	{
	case 0:
		Byte[0] = inb(0x60);
		LeftButton = Byte[0] & 1;
		RightButton = Byte[0] & 2;
		Cycle++;
		break;
	case 1:
		Byte[1] = inb(0x60);
		Cycle++;
		break;
	case 2:
		Byte[2] = inb(0x60);
		AccX = Byte[1];
		AccY = -Byte[2];
		Cycle = 0;
		break;
	}

	MouseX += AccX;
	MouseY += AccY;

	if (MouseX >= 800) MouseX = 799;
	if (MouseY >= 600) MouseY = 599;
	if (MouseX < 0) MouseX = 0;
	if (MouseY < 0) MouseY = 0;

	MousePrevX = MouseX;
	MousePrevY = MouseY;
}

static inline void Wait(unsigned char type)
{
	unsigned int timeout = 100000;
	if(type == 0)
	{
		while(timeout--) if((inb(0x64) & 1)==1) return;
		return;
	}
	else
	{
		while(timeout--) if((inb(0x64) & 2) == 0) return;
		return;
	}
}

static inline void Write(unsigned char data)
{
	Wait(1);
	outb(0x64, 0xD4);
	Wait(1);
	outb(0x60, data);
}

static char Read()
{
	Wait(0);
	return inb(0x60);
}

int Mouse::GetX()
{
	return MouseX;
}

int Mouse::GetY()
{
	return MouseY;
}

int Mouse::GetLeft()
{
	return LeftButton;
}

int Mouse::GetRight()
{
	return RightButton;
}

void Mouse::Init()
{
	unsigned char status;

	Wait(1);
	outb(0x64, 0xA8);

	Wait(1);
	outb(0x64, 0x20);
	Wait(0);
	status = (inb(0x60) | 2);
	Wait(1);
	outb(0x64, 0x60);
	Wait(1);
	outb(0x60, status);

	Write(0xF6);
	Read();

	Write(0xF4);
	Read();

	IRQ::InstallHandler(12, Handler);
}
