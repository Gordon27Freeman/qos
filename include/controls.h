#ifndef CONTROLS_H
#define CONTROLS_H

struct Button
{
	char *label;
	int x, y, w, h;
	char pressed, lastPressed;
	void (*onClick)();
};

namespace Controls
{
	unsigned int *CreateButton(char *label, int x, int y, int w, int h);
	void DestroyButton(unsigned int *button);
	void DrawButton(unsigned int *buffer, int bw, int bh, unsigned int *button);
	void ClickButton(unsigned int *button, int x, int y);
	void ReleaseButton(unsigned int *button, int x, int y);
}

#endif
