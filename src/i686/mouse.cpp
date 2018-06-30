#include <i686/mouse.h>
#include <i686/graphics.h>
#include <i686/regs.h>
#include <i686/irq.h>
#include <i686/pio.h>
using namespace Mouse;

static unsigned int *framebuffer;

static int MouseX = 400, MouseY = 300, MousePrevX = MouseX, MousePrevY = MouseY;
static char Cycle = 0, Byte[3], AccX, AccY;

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
	unsigned char status;
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

	if (MouseX < 800 && MouseX > -1) MouseX += AccX;
	if (MouseY < 600 && MouseY > -1) MouseY += AccY;
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
				framebuffer[cx + cy * 800] = 0xFFFFFF;
				break;
			case 2:
				framebuffer[cx + cy * 800] = 0;
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

void Mouse::Init()
{
	unsigned char status;

	outb(0x64, 0xA8);
	outb(0x64, 0x20);
	status = (inb(0x60) | 2);
	outb(0x64, 0x60);
	outb(0x60, status);

	outb(0x64, 0xD4);
	outb(0x60, 0xF6);
	inb(0x60);

	outb(0x64, 0xD4);
	outb(0x60, 0xF4);
	inb(0x60);

	framebuffer = Graphics::GetFramebuffer();
	Graphics::GetBuffer(Buffer, MouseX, MouseY, 11, 16);
	IRQ::InstallHandler(12, Handler);
}
