#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace Graphics
{
	void Init(unsigned long long addr);
	void Line(unsigned int *buffer, int bufferWidth, int x0, int y0, int x1, int y1, unsigned int color);
	void HorizontalLine(unsigned int *buffer, int bufferWidth, int x, int y, int length, unsigned int color);
	void VerticalLine(unsigned int *buffer, int bufferWidth, int x, int y, int length, unsigned int color);
	void Rect(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int color);
	void FillRect(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int color);
	void DrawBuffer(unsigned int *dest, unsigned int *buffer, int dw, int dh, int x, int y, int w, int h);
	void DrawFullscreenBuffer(unsigned int *buffer);
	void DrawChar(unsigned int *buffer, int bufferWidth, char c, int x, int y, unsigned int color);
	void DrawString(unsigned int *buffer, int bufferWidth, const char *s, int x, int y, unsigned int color);
	void HorizontalGradient(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int startColor, unsigned int endColor);
	void VerticalGradient(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int startColor, unsigned int endColor);
}

#endif
