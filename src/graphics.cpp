#include <graphics.h>
#include <memory.h>
using namespace Graphics;

static unsigned int *framebuffer;

void Graphics::Init(unsigned long long addr)
{
	framebuffer = (unsigned int *)addr;
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
