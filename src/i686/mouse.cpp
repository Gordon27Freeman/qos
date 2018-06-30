#include <i686/mouse.h>
#include <i686/regs.h>
#include <i686/irq.h>
#include <i686/pio.h>

#include <graphics.h>

using namespace Mouse;

static unsigned int *framebuffer;

static int MouseX = 400, MouseY = 300, MousePrevX = MouseX, MousePrevY = MouseY;
static char Cycle = 0, Byte[3];

static unsigned int Cursor[11 * 16] =
{
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
	1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
	1, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 1, 0, 1, 2, 2, 1, 0, 0, 0,
	1, 1, 0, 0, 1, 2, 2, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0
};

static unsigned int Buffer[11 * 16];

static void Handler(struct regs *r)
{
	register char AccX = 0, AccY = 0;

	switch (Cycle)
	{
	case 0:
		Byte[0] = inb(0x60);
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

	Graphics::DrawBuffer(Buffer, MousePrevX, MousePrevY, 11, 16);
	Graphics::GetBuffer(Buffer, MouseX, MouseY, 11, 16);

	register int cy = MouseY, cx = MouseX, ly = MouseY + 16, lx = MouseX + 11;
	while (cy < ly)
	{
		while (cx < lx)
		{
			switch (Cursor[(cx - MouseX) + (cy - MouseY) * 11])
			{
			case 1:
				if (cx < 800) framebuffer[cx + cy * 800] = 0;
				break;
			case 2:
				if (cx < 800) framebuffer[cx + cy * 800] = 0xFFFFFF;
				break;
			}
			cx++;
		}
		cx = MouseX;
		cy++;
	}

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

	framebuffer = Graphics::GetFramebuffer();
	Graphics::GetBuffer(Buffer, MouseX, MouseY, 11, 16);
	IRQ::InstallHandler(12, Handler);
}
