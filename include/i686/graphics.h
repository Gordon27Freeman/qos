#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace Graphics
{
	unsigned int *GetFramebuffer();
	void Init(unsigned long long addr);
	void HorizontalLine(unsigned int *buffer, int bufferWidth, int x, int y, int length, unsigned int color);
	void VerticalLine(unsigned int *buffer, int bufferWidth, int x, int y, int length, unsigned int color);
	void Rect(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int color);
	void FillRect(unsigned int *buffer, int bufferWidth, int x, int y, int w, int h, unsigned int color);
	void DrawBuffer(unsigned int *buffer, int x, int y, int w, int h);
	void GetBuffer(unsigned int *buffer, int x, int y, int w, int h);
	void DrawFullscreenBuffer(unsigned int *buffer);
	void DrawChar(unsigned int *buffer, int bufferWidth, char c, int x, int y, unsigned int color);
	void DrawString(unsigned int *buffer, int bufferWidth, char *s, int x, int y, unsigned int color);
}

#endif
