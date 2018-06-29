#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace Graphics
{
	unsigned int *GetFramebuffer();
	void Init(unsigned long long addr);
	void HorizontalLine(int x, int y, int length, unsigned int color);
	void VerticalLine(int x, int y, int length, unsigned int color);
	void Rect(int x, int y, int w, int h, unsigned int color);
	void FillRect(int x, int y, int w, int h, unsigned int color);
	void DrawBuffer(unsigned int *buffer, int x, int y, int w, int h);
	void GetBuffer(unsigned int *buffer, int x, int y, int w, int h);
	void DrawFullscreenBuffer(unsigned int *buffer);
}

#endif
