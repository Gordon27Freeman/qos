#ifndef CONTROLS_H
#define CONTROLS_H

struct Label
{
	char *text;
	int x, y;
	char visible, active;
};

struct Button
{
	char *label;
	int x, y, w, h;
	char pressed, lastPressed;
	char visible, active;
	void (*onClick)();
};

struct TextBox
{
	char *text;
	int x, y, w, h;
	char focus;
	char visible, active;
};

namespace Controls
{
	unsigned int *CreateLabel(char *text, int x, int y);
	void DestroyLabel(unsigned int *label);
	void DrawLabel(unsigned int *buffer, int bw, int bh, unsigned int *label);

	unsigned int *CreateButton(char *label, int x, int y, int w, int h);
	void DestroyButton(unsigned int *button);
	void DrawButton(unsigned int *buffer, int bw, int bh, unsigned int *button);
	void ClickButton(unsigned int *button, int x, int y);
	void ReleaseButton(unsigned int *button, int x, int y);

	unsigned int *CreateTextBox(char *text, int x, int y, int w, int h);
	void DrawTextBox(unsigned int *buffer, int bw, int bh, unsigned int *textbox);
	void DestroyTextBox(unsigned int *textbox);
}

#endif
