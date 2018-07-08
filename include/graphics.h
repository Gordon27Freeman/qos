#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace Graphics
{
	void Init(unsigned long long addr);
	void Line(unsigned int *buffer, int bw, int bh, int x0, int y0, int x1, int y1, unsigned int color);
	void HorizontalLine(unsigned int *buffer, int bw, int bh, int x, int y, int length, unsigned int color);
	void VerticalLine(unsigned int *buffer, int bw, int bh, int x, int y, int length, unsigned int color);
	void Rect(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int color);
	void FillRect(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int color);
	void DrawFullscreenBuffer(unsigned int *buffer);
	void DrawChar(unsigned int *buffer, int bw, int bh, char c, int x, int y, unsigned int color);
	void DrawString(unsigned int *buffer, int bw, int bh, const char *s, int x, int y, unsigned int color);
	void HorizontalGradient(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int startColor, unsigned int endColor);
	void VerticalGradient(unsigned int *buffer, int bw, int bh, int x, int y, int w, int h, unsigned int startColor, unsigned int endColor);
}

#endif
