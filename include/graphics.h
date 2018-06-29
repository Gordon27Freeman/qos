#ifndef GRAPHICS_H
#define GRAPHICS_H

namespace Graphics
{
	void Init(unsigned long long addr);
	void HorizontalLine(int x, int y, int length, unsigned int color);
	void VerticalLine(int x, int y, int length, unsigned int color);
	void Rect(int x, int y, int w, int h, unsigned int color);
	void FillRect(int x, int y, int w, int h, unsigned int color);
}

#endif
