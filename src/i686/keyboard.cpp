#include <keyboard.h>
#include <regs.h>
#include <pio.h>
#include <irq.h>
using namespace Keyboard;

unsigned char scancodes[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

unsigned char scancodesShift[128] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	'(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0,
	'|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

unsigned char scancodesCaps[128] =
{
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0,
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', 0,
	'\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

unsigned char scancodesCapsShift[128] =
{
	0,  27, '!', '@', '#', '$', '%', '^', '&', '*',	'(', ')', '_', '+', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '{', '}', '\n', 0,
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '"', '~', 0,
	'|', 'z', 'x', 'c', 'v', 'b', 'n', 'm', '<', '>', '?', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	0, 0, 0, '-',
	0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

char shift = 0, caps = 0, alt = 0, control = 0, lastKey = 0;

static void Handler(struct regs *r)
{
	unsigned char scancode;
	scancode = inb(0x60);

	if (scancode & 0x80)
	{
		switch(scancode)
		{	
			case 0xaa: shift = 0; break;
			case 0xb6: shift = 0; break;
			case 0x9d: control = 0; break;
			case 0xb8: alt = 0; break;
			default: break;
		}
	}
	else
	{
		switch(scancode)
		{
			case 0x00: break; case 0x01: break; case 0x3b: break;
			case 0x3c: break; case 0x3d: break; case 0x3e: break;
			case 0x3f: break; case 0x40: break; case 0x41: break;
			case 0x42: break; case 0x43: break; case 0x44: break;
			case 0x45: break; case 0x46: break; case 0x47: break;
			case 0x48: break; case 0x49: break; case 0x4b: break;
			case 0x4d: break; case 0x4f: break; case 0x50: break;
			case 0x51: break; case 0x52: break; case 0x53: break;
			case 0x57: break; case 0x58: break;
			
			case 0x2a: shift = 1; break;
			case 0x36: shift = 1; break;
			case 0x1d: control = 1; break;
			case 0x38: alt = 1; break;
			case 0x3a: if(!caps) caps = 1; else caps = 0; break;
			
			default:
				if(caps && shift) lastKey = scancodesCapsShift[scancode];
				else if(shift) lastKey = scancodesShift[scancode];
				else if(caps) lastKey = scancodesCaps[scancode];
				else lastKey = scancodes[scancode];
				break;
		}
	}
}

char Keyboard::GetChar()
{
	char c = lastKey;
	lastKey = 0;
	return c;
}

void Keyboard::Init()
{
	IRQ::InstallHandler(1, Handler);
}
