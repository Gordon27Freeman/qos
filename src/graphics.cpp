#include <graphics.h>
#include <memory.h>
#include <font.h>
using namespace Graphics;

static int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
static unsigned int *framebuffer;
extern struct Font VGAFont;

unsigned int *Graphics::GetFramebuffer()
{
	return framebuffer;
}

void Graphics::Init(unsigned long long addr)
{
	framebuffer = (unsigned int *)addr;
	register unsigned int cx = 0;
	while (cx < 800 * 600)
	{
		framebuffer[cx] = 0x007b7b;
		cx++;
	}
}

void Graphics::HorizontalLine(unsigned int *buffer, int bufferWidth, int x, int y, int length, unsigned int color)
{
	register unsigned int cx = x, l = x + length;
	while (cx < l)
	{
		buffer[cx + y * bufferWidth] = color;
		cx++;
	}
}

void Graphics::VerticalLine(unsigned int *buffer, int bufferWidth, int x, int y, int length, unsigned int color)
{
	register int cy = y, l = y + length;
	while (cy < l)
	{
		buffer[x + cy * bufferWidth] = color;
		cy++;
	}
}

void Graphics::Rect(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int color)
{
	HorizontalLine(buffer, bufferWidth, x, y, w, color);
	HorizontalLine(buffer, bufferWidth, x, y + h, w, color);
	VerticalLine(buffer, bufferWidth, x, y, h, color);
	VerticalLine(buffer, bufferWidth, x + w, y, h, color);
}

void Graphics::FillRect(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int color)
{
	register int cy = y, cx = x, ly = y + h, lx = x + w;
	while (cy < ly)
	{
		while (cx < lx)
		{
			buffer[cx + cy * bufferWidth] = color;
			cx++;
		}
		cx = x;
		cy++;
	}
}

void Graphics::DrawBuffer(unsigned int *buffer, int x, int y, int w, int h)
{
	register int cy = y, l = y + h;
	while (cy < l)
	{
		memcpy((void *)&framebuffer[x + cy * 800], (void *)&buffer[(cy - y) * w], w * 4);
		cy++;
	}
}

void Graphics::GetBuffer(unsigned int *buffer, int x, int y, int w, int h)
{
	register int cy = y, l = y + h;
	while (cy < l)
	{
		memcpy((void *)&buffer[(cy - y) * w], (void *)&framebuffer[x + cy * 800], w * 4);
		cy++;
	}
}

void Graphics::DrawFullscreenBuffer(unsigned int *buffer)
{
	memcpy((void *)&framebuffer[0], (void *)&buffer[0], 800 * 600 * 4);
}

void Graphics::DrawChar(unsigned int *buffer, int bufferWidth, char c, int x, int y, unsigned int color)
{
	register int cx = 0, cy = 0;
	register unsigned char *glyph = &VGAFont.Bitmap[(c - 31) * 16];
 
	while(cy < 16)
	{
		while(cx < 8)
		{
			if (glyph[cy] & mask[cx]) buffer[(x + cx) + (y + cy) * bufferWidth] = color;
			cx++;
		}
		cx = 0;
		cy++;
	}
}

void Graphics::DrawString(unsigned int *buffer, int bufferWidth, char *s, int x, int y, unsigned int color)
{
	register int cx = 0;
	while(*s)
	{
		DrawChar(buffer, bufferWidth, *s, x + cx, y, color);
		s++;
		cx += 8;
	}
}
