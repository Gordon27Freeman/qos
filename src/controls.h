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
	int length;
	char focus;
	char visible, active;
};

struct CheckBox
{
	char *label;
	int x, y;
	char checked, lastPressed;
	char visible, active;
};

struct RadioButton
{
	char *label;
	int x, y;
	char marked, chanel, lastChanel, init;
	char visible, active;
};

namespace Controls
{
	unsigned int *CreateLabel(char *text, int x, int y);
	void DestroyLabel(unsigned int *label);
	void DrawLabel(unsigned int *buffer, int bw, int bh, unsigned int *label, int x, int y);

	unsigned int *CreateButton(char *label, int x, int y, int w, int h);
	void DestroyButton(unsigned int *button);
	void DrawButton(unsigned int *buffer, int bw, int bh, unsigned int *button, int x, int y);
	void ClickButton(unsigned int *button, int x, int y);
	void ReleaseButton(unsigned int *button, int x, int y);

	unsigned int *CreateTextBox(char *text, int x, int y, int w, int h);
	void DestroyTextBox(unsigned int *textbox);
	void DrawTextBox(unsigned int *buffer, int bw, int bh, unsigned int *textbox, int x, int y);
	void ClickTextBox(unsigned int *textbox, int x, int y);
	void KeyPressTextBox(unsigned int *textbox, char c);

	unsigned int *CreateCheckBox(char *label, int x, int y);
	void DestroyCheckBox(unsigned int *checkbox);
	void DrawCheckBox(unsigned int *buffer, int bw, int bh, unsigned int *checkbox, int x, int y);
	void ClickCheckBox(unsigned int *checkbox, int x, int y);
	void ReleaseCheckBox(unsigned int *checkbox, int x, int y);

	unsigned int *CreateRadioButton(char *label, int x, int y);
	void DestroyRadioButton(unsigned int *radio);
	void DrawRadioButton(unsigned int *buffer, int bw, int bh, unsigned int *radio, int x, int y);
	void ClickRadioButton(unsigned int *radio, int x, int y);
}

#endif
