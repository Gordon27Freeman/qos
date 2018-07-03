#include <graphics.h>
#include <font.h>
#include <mm.h>

#ifdef ARCH_i686
#include <i686/memory.h>
#endif

using namespace Graphics;

static int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
static unsigned int *framebuffer;
extern struct Font VGAFont;

void Graphics::Init(unsigned long long addr)
{
	framebuffer = (unsigned int *)addr;
	int cx = 0;
	while (cx < 800 * 600)
	{
		framebuffer[cx] = 0x007b7b;
		cx++;
	}
}

void Graphics::SetPixel(unsigned int *buffer, int bw, int bh, int x, int y, unsigned int color)
{
	if (x < bw && y < bh) buffer[x + y * bw] = color;
}

void Graphics::Line(unsigned int *buffer, int bw, int bh, int x0, int y0, int x1, int y1, unsigned int color)
{
	int dx, dy;
	if (x1 >= x0) dx = x1 - x0;
	else dx = x0 - x1;
	if (y1 >= y0) dy = y1 - y0;
	else dy = y0 - y1;

	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1; 
	int err = (dx > dy ? dx : -dy) / 2, e2;
 
	while (x0 != x1 && y0 != y1)
	{
		buffer[x0 + y0 * bw] = color;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
	buffer[x0 + y0 * bw] = color;
}

void Graphics::HorizontalLine(unsigned int *buffer, int bw, int bh, int x, int y, int length, unsigned int color)
{
	int cx = x, l = x + length;
	if (y < bh) while (cx < l && cx < bw)
	{
		buffer[cx + y * bw] = color;
		cx++;
	}
}

void Graphics::VerticalLine(unsigned int *buffer, int bw, int bh, int x, int y, int length, unsigned int color)
{
	int cy = y, l = y + length;
	if (x < bw) while (cy < l)
	{
		buffer[x + cy * bw] = color;
		cy++;
	}
}

void Graphics::Rect(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int color)
{
	HorizontalLine(buffer, bw, bh, x, y, w, color);
	HorizontalLine(buffer, bw, bh, x, y + h, w, color);
	VerticalLine(buffer, bw, bh, x, y, h, color);
	VerticalLine(buffer, bw, bh, x + w, y, h, color);
}

void Graphics::FillRect(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int color)
{
	int cy = y, cx = x, ly = y + h, lx = x + w;
	while (cy < ly && cy < bh)
	{
		while (cx < lx && cx < bw)
		{
			buffer[cx + cy * bw] = color;
			cx++;
		}
		cx = x;
		cy++;
	}
}

void Graphics::DrawBuffer(unsigned int *dest, unsigned int *buffer, int bw, int bh, int x, int y, int w, int h)
{
	int cy = y, l = y + h, width;
	if ((x + w) < bw) width = w * 4;
	else width = (bw * 4) % (x * 4);
	while (cy < l && cy < bh)
	{
		for(int cx = x; cx < (x + w); cx++)
			if(cx < bw && cx > -1 && cy > -1) dest[cx + cy * bw] = buffer[(cx - x) + (cy - y) * w];
		cy++;
	}
}

void Graphics::DrawFullscreenBuffer(unsigned int *buffer)
{
	memcpy((void *)&framebuffer[0], (void *)&buffer[0], 800 * 600 * 4);
}

void Graphics::DrawChar(unsigned int *buffer, int bw, int bh, char c, int x, int y, unsigned int color)
{
	int cx = 0, cy = 0;
	unsigned char *glyph = &VGAFont.Bitmap[(c - 31) * 16];
 
	while(cy < 16 && (y + cy) < bh)
	{
		while(cx < 8 && (x + cx) < bw)
		{
			if (glyph[cy] & mask[cx] && (x + cx) > 0)
				buffer[(x + cx) + (y + cy) * bw] = color;
			cx++;
		}
		cx = 0;
		cy++;
	}
}

void Graphics::DrawString(unsigned int *buffer, int bw, int bh, const char *s, int x, int y, unsigned int color)
{
	int cx = 0;
	while(*s)
	{
		DrawChar(buffer, bw, bh, *s, x + cx, y, color);
		s++;
		cx += 8;
	}
}

void Graphics::HorizontalGradient(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int startColor, unsigned int endColor)
{
	float ecb = endColor & 0xFF;
	float ecg = (endColor >> 8) & 0xFF;
	float ecr = (endColor >> 16) & 0xFF;
	int cx = x, cy = y, lx = x + w, ly = y + h;
	while (cy < ly && cy < bh)
	{
		float scb = startColor & 0xFF;
		float scg = (startColor >> 8) & 0xFF;
		float scr = (startColor >> 16) & 0xFF;
		while (cx < lx && cx < bw)
		{
			if (scb < ecb) scb += (ecb - scb) / w;
			else if (scb > ecb) scb -= (scb - ecb) / w;
			if (scg < ecg) scg += (ecg - scg) / w;
			else if (scg > ecg) scg -= (scg - ecg) / w;
			if (scr < ecr) scr += (ecr - scr) / w;
			else if (scr > ecr) scr -= (scr - ecr) / w;

			buffer[cx + cy * bw] = (int)scb + ((int)scg << 8) + ((int)scr << 16);
			cx++;
		}
		cx = x;
		cy++;
	}
}

void Graphics::VerticalGradient(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int startColor, unsigned int endColor)
{
	float ecb = endColor & 0xFF;
	float ecg = (endColor >> 8) & 0xFF;
	float ecr = (endColor >> 16) & 0xFF;
	int cx = x, cy = y, lx = x + w, ly = y + h;
	while (cx < lx && cx < bw)
	{
		float scb = startColor & 0xFF;
		float scg = (startColor >> 8) & 0xFF;
		float scr = (startColor >> 16) & 0xFF;
		while (cy < ly && cy < bh)
		{
			if (scb < ecb) scb += (ecb - scb) / w;
			else if (scb > ecb) scb -= (scb - ecb) / w;
			if (scg < ecg) scg += (ecg - scg) / w;
			else if (scg > ecg) scg -= (scg - ecg) / w;
			if (scr < ecr) scr += (ecr - scr) / w;
			else if (scr > ecr) scr -= (scr - ecr) / w;

			buffer[cx + cy * bw] = (int)scb + ((int)scg << 8) + ((int)scr << 16);
			cy++;
		}
		cx++;
		cy = y;
	}
}
