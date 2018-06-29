#include <graphics.h>
#include <memory.h>
using namespace Graphics;

static unsigned int *framebuffer;

unsigned int *Graphics::GetFramebuffer()
{
	return framebuffer;
}

void Graphics::Init(unsigned long long addr)
{
	framebuffer = (unsigned int *)addr;
	memset(framebuffer, 0xd0d0d0, 800 * 600 * 4);
}

void Graphics::HorizontalLine(int x, int y, int length, unsigned int color)
{
	memset((void *)&framebuffer[x + y * 800], color, length * 4);
}

void Graphics::VerticalLine(int x, int y, int length, unsigned int color)
{
	register int cy = y, l = y + length;
	while (cy < l)
	{
		framebuffer[x + cy * 800] = color;
		cy++;
	}
}

void Graphics::Rect(int x, int y, int w, int h, unsigned int color)
{
	HorizontalLine(x, y, w, color);
	HorizontalLine(x, y + h, w, color);
	VerticalLine(x, y, h, color);
	VerticalLine(x + w, y, h, color);
}

void Graphics::FillRect(int x, int y, int w, int h, unsigned int color)
{
	register int cy = y, l = y + h;
	while (cy < l)
	{
		memset((void *)&framebuffer[x + cy * 800], color, w * 4);
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
