#include <graphics.h>
#include <font.h>
#include <mm.h>

#ifdef ARCH_i686
#include <i686/memory.h>
#endif

using namespace Graphics;

static int mask[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
unsigned int *framebuffer;
extern struct Font VGAFont;

void Graphics::Init(unsigned long long addr)
{
	framebuffer = (unsigned int *)addr;
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
		if (x0 >= 0 && y0 >= 0 && x0 < bw && y0 < bh) buffer[x0 + y0 * bw] = color;
		e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
	if (x0 >= 0 && y0 >= 0 && x0 < bw && y0 < bh) buffer[x0 + y0 * bw] = color;
}

void Graphics::Circle(unsigned int *buffer, int bw, int bh, int x0, int y0, int r, unsigned int color)
{
	int x = r - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (r << 1);

	while (x >= y)
	{
		if ((x0 + x) >= 0 && (y0 + y) >= 0 && (x0 + x) < bw && (y0 + y) < bh) buffer[(x0 + x) + (y0 + y) * bw] = color;
		if ((x0 + y) >= 0 && (y0 + x) >= 0 && (x0 + y) < bw && (y0 + x) < bh) buffer[(x0 + y) + (y0 + x) * bw] = color;
		if ((x0 - y) >= 0 && (y0 + x) >= 0 && (x0 - y) < bw && (y0 + x) < bh) buffer[(x0 - y) + (y0 + x) * bw] = color;
		if ((x0 - x) >= 0 && (y0 + y) >= 0 && (x0 - x) < bw && (y0 + y) < bh) buffer[(x0 - x) + (y0 + y) * bw] = color;
		if ((x0 - x) >= 0 && (y0 - y) >= 0 && (x0 - x) < bw && (y0 - y) < bh) buffer[(x0 - x) + (y0 - y) * bw] = color;
		if ((x0 - y) >= 0 && (y0 - x) >= 0 && (x0 - y) < bw && (y0 - x) < bh) buffer[(x0 - y) + (y0 - x) * bw] = color;
		if ((x0 + y) >= 0 && (y0 - x) >= 0 && (x0 + y) < bw && (y0 - x) < bh) buffer[(x0 + y) + (y0 - x) * bw] = color;
		if ((x0 + x) >= 0 && (y0 - y) >= 0 && (x0 + x) < bw && (y0 - y) < bh) buffer[(x0 + x) + (y0 - y) * bw] = color;

		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
        
		if (err > 0)
		{
			x--;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}

void Graphics::FillCircle(unsigned int *buffer, int bw, int bh, int x0, int y0, int r, unsigned int color)
{
	int x = r;
    int y = 0;
    int xChange = 1 - (r << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = x0 - x; i <= x0 + x; i++)
        {
            if (i >= 0 && (y0 + y) >= 0 && i < bw && (y0 + y) < bh) buffer[i + (y0 + y) * bw] = color;
            if (i >= 0 && (y0 - y) >= 0 && i < bw && (y0 - y) < bh) buffer[i + (y0 - y) * bw] = color;
        }
        for (int i = x0 - y; i <= x0 + y; i++)
        {
            if (i >= 0 && (y0 + x) >= 0 && i < bw && (y0 + x) < bh) buffer[i + (y0 + x) * bw] = color;
            if (i >= 0 && (y0 - x) >= 0 && i < bw && (y0 - x) < bh) buffer[i + (y0 - x) * bw] = color;
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

void Graphics::HorizontalLine(unsigned int *buffer, int bw, int bh, int x, int y, int length, unsigned int color)
{
	int cx = x, l = x + length;
	if (y < bh) while (cx < l && cx < bw)
	{
		if (cx >= 0 && y >= 0) buffer[cx + y * bw] = color;
		cx++;
	}
}

void Graphics::VerticalLine(unsigned int *buffer, int bw, int bh, int x, int y, int length, unsigned int color)
{
	int cy = y, l = y + length;
	if (x < bw) while (cy < l && cy < bh)
	{
		if (x >= 0 && cy >= 0) buffer[x + cy * bw] = color;
		cy++;
	}
}

void Graphics::Rect(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int color)
{
	HorizontalLine(buffer, bw, bh, x, y, w, color);
	VerticalLine(buffer, bw, bh, x, y, h, color);
	HorizontalLine(buffer, bw, bh, x, y + h, w + 1, color);
	VerticalLine(buffer, bw, bh, x + w, y, w, color);
}

void Graphics::FillRect(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int color)
{
	int cy = y, cx = x, ly = y + h, lx = x + w;
	while (cy < ly && cy < bh)
	{
		while (cx < lx && cx < bw)
		{
			if (cx >= 0 && cy >= 0) buffer[cx + cy * bw] = color;
			cx++;
		}
		cx = x;
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
			if (glyph[cy] & mask[cx] && (x + cx) >= 0 && (y + cy) >= 0)
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

			if (cx < bw && cx >= 0 && cy < bh && cy >= 0) buffer[cx + cy * bw] = (int)scb + ((int)scg << 8) + ((int)scr << 16);
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

			if (cx < bw && cx >= 0 && cy < bh && cy >= 0) buffer[cx + cy * bw] = (int)scb + ((int)scg << 8) + ((int)scr << 16);
			cy++;
		}
		cx++;
		cy = y;
	}
}
