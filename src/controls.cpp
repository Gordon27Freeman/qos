#include <controls.h>
#include <graphics.h>
#include <string.h>
#include <mm.h>

using namespace Controls;

unsigned int *Controls::CreateLabel(char *text, int x, int y)
{
	struct Label *lbl = (struct Label *)Memory::Alloc(sizeof(struct Label));
	lbl->text = text;
	lbl->x = x;
	lbl->y = y;
	lbl->active = 1;
	lbl->visible = 1;
	return (unsigned int *)lbl;
}

void Controls::DestroyLabel(unsigned int *label)
{
	struct Label *lbl = (struct Label *)label;
	Memory::Free((void *)lbl);
}

void Controls::DrawLabel(unsigned int *buffer, int bw, int bh, unsigned int *label, int x, int y)
{
	// its label so basically draw text with color depending on its activity
	struct Label *lbl = (struct Label *)label;
	int color;
	if (lbl->active) color = 0x101010; else color = 0x806050;
	if (lbl->visible) Graphics::DrawString(buffer, bw, bh, lbl->text, x + lbl->x + 8, y + lbl->y + 27, color);
}

unsigned int *Controls::CreateButton(char *label, int x, int y, int w, int h)
{
	struct Button *btn = (struct Button *)Memory::Alloc(sizeof(struct Button));
	btn->label = label;
	btn->x = x;
	btn->y = y;
	btn->w = w;
	btn->h = h;
	btn->active = 1;
	btn->visible = 1;
	return (unsigned int *)btn;
}

void Controls::DestroyButton(unsigned int *button)
{
	struct Button *btn = (struct Button *)button;
	Memory::Free((void *)btn);
}

void Controls::DrawButton(unsigned int *buffer, int bw, int bh, unsigned int *button, int x, int y)
{
	// button is a bit more complicated
	struct Button *btn = (struct Button *)button;
	int rightBottom, leftTop, color, textColor;

	if (btn->active) textColor = 0x101010;
	else textColor = 0x806050;

	// also need to change color if pressed
	if (!btn->pressed)
	{
		rightBottom = 0x907060;
		leftTop = 0xf0c0b0;
		color = 0xb0a090;
	}
	else
	{
		rightBottom = 0xf0c0b0;
		leftTop = 0x907060;
		color = 0xa09080;
	}

	// draw 3d rect with text on it
	if (btn->visible)
	{
		Graphics::FillRect(buffer, bw, bh, x + btn->x + 4, y + btn->y + 24, btn->w, btn->h, color);
		Graphics::HorizontalLine(buffer, bw, bh, x + btn->x + 4, y + btn->y + 24, btn->w, leftTop);
		Graphics::VerticalLine(buffer, bw, bh, x + btn->x + 4, y + btn->y + 24, btn->h, leftTop);
		Graphics::HorizontalLine(buffer, bw, bh, x + btn->x + 4, y + btn->y + btn->h + 24, btn->w + 1, rightBottom);
		Graphics::VerticalLine(buffer, bw, bh, x + btn->x + btn->w + 4, y + btn->y + 24, btn->h, rightBottom);
		Graphics::DrawString(buffer, bw, bh, btn->label, x + btn->x + 8, y + btn->y + 27, textColor);
	}
}

void Controls::ClickButton(unsigned int *button, int x, int y)
{
	struct Button *btn = (struct Button *)button;
	if (x >= btn->x && x <= btn->x + btn->w && y >= btn->y && y <= btn->y + btn->h)
	{
		if (btn->lastPressed == 0 && btn->onClick && btn->active) btn->onClick();
		btn->pressed = 1;
		btn->lastPressed = 1;
	}
}

void Controls::ReleaseButton(unsigned int *button, int x, int y)
{
	struct Button *btn = (struct Button *)button;
	if (btn->active)
	{
		btn->pressed = 0;
		btn->lastPressed = 0;
	}
}

unsigned int *Controls::CreateTextBox(char *text, int x, int y, int w, int h)
{
	// allocate some memory for text in textbox cause it can be changed
	struct TextBox *txt = (struct TextBox *)Memory::Alloc(sizeof(struct TextBox));
	txt->text = (char *)Memory::Alloc(65536);
	strcpy(txt->text, text);
	txt->length = 7;
	txt->x = x;
	txt->y = y;
	txt->w = w;
	txt->h = h;
	txt->active = 1;
	txt->visible = 1;
	return (unsigned int *)txt;
}

void Controls::DestroyTextBox(unsigned int *textbox)
{
	struct TextBox *txt = (struct TextBox *)textbox;
	Memory::Free((void *)txt->text);
	Memory::Free((void *)txt);
}

void Controls::DrawTextBox(unsigned int *buffer, int bw, int bh, unsigned int *textbox, int x, int y)
{
	struct TextBox *txt = (struct TextBox *)textbox;

	int color;
	if (txt->active) color = 0xffffff;
	else color = 0xb0a090;

	if (txt->visible)
	{
		// cut text to fit textbox size, draw only its end
		char *ptr;
		if (txt->length > (txt->w - 8) / 8) ptr = (char *)&txt->text[txt->length - (txt->w - 8) / 8];
		else ptr = txt->text;
		
		Graphics::FillRect(buffer, bw, bh, x + txt->x + 4, y + txt->y + 24, txt->w, txt->h, color);
		Graphics::HorizontalLine(buffer, bw, bh, x + txt->x + 4, y + txt->y + 24, txt->w, 0x907060);
		Graphics::VerticalLine(buffer, bw, bh, x + txt->x + 4, y + txt->y + 24, txt->h, 0x907060);
		Graphics::HorizontalLine(buffer, bw, bh, x + txt->x + 4, y + txt->y + txt->h + 24, txt->w + 1, 0xf0c0b0);
		Graphics::VerticalLine(buffer, bw, bh, x + txt->x + txt->w + 4, y + txt->y + 24, txt->h, 0xf0c0b0);
		Graphics::DrawString(buffer, bw, bh, ptr, x + txt->x + 8, y + txt->y + 27, 0x101010);
	}
}

void Controls::ClickTextBox(unsigned int *textbox, int x, int y)
{
	// set focus on mouse click
	struct TextBox *txt = (struct TextBox *)textbox;
	if (x >= txt->x && x <= txt->x + txt->w && y >= txt->y && y <= txt->y + txt->h) txt->focus = 1;
	else txt->focus = 0;
}

void Controls::KeyPressTextBox(unsigned int *textbox, char c)
{
	// if in focus add pressed keys to textbox
	struct TextBox *txt = (struct TextBox *)textbox;
	if (txt->active && txt->focus)
	{
		if (c == '\b')
		{
			if (txt->length > 0)
			{
				txt->length--;
				txt->text[txt->length] = 0;
			}
		}
		else
		{
			txt->text[txt->length] = c;
			txt->length++;
		}
	}
}

unsigned int *Controls::CreateCheckBox(char *label, int x, int y)
{
	struct CheckBox *chk = (struct CheckBox *)Memory::Alloc(sizeof(struct CheckBox));
	chk->label = label;
	chk->x = x;
	chk->y = y;
	chk->active = 1;
	chk->visible = 1;
	return (unsigned int *)chk;
}

void Controls::DestroyCheckBox(unsigned int *checkbox)
{
	struct CheckBox *chk = (struct CheckBox *)checkbox;
	Memory::Free((void *)chk);
}

void Controls::DrawCheckBox(unsigned int *buffer, int bw, int bh, unsigned int *checkbox, int x, int y)
{
	struct CheckBox *chk = (struct CheckBox *)checkbox;

	int color;
	if (chk->active) color = 0xffffff;
	else color = 0xb0a090;

	if (chk->visible)
	{
		Graphics::FillRect(buffer, bw, bh, x + chk->x + 8, y + chk->y + 28, 11, 11, color);
		Graphics::HorizontalLine(buffer, bw, bh, x + chk->x + 8, y + chk->y + 28, 11, 0x907060);
		Graphics::VerticalLine(buffer, bw, bh, x + chk->x + 8, y + chk->y + 28, 11, 0x907060);
		Graphics::HorizontalLine(buffer, bw, bh, x + chk->x + 8, y + chk->y + 39, 12, 0xf0c0b0);
		Graphics::VerticalLine(buffer, bw, bh, x + chk->x + 19, y + chk->y + 28, 12, 0xf0c0b0);
		Graphics::DrawString(buffer, bw, bh, chk->label, x + chk->x + 24, y + chk->y + 27, 0x101010);

		// draw cute check mark
		if (chk->checked)
		{
			Graphics::Line(buffer, bw, bh, x + chk->x + 17, y + chk->y + 30, x + chk->x + 13, y + chk->y + 38, 0x101010);
			Graphics::Line(buffer, bw, bh, x + chk->x + 18, y + chk->y + 30, x + chk->x + 14, y + chk->y + 38, 0x101010);
			Graphics::Line(buffer, bw, bh, x + chk->x + 9, y + chk->y + 34, x + chk->x + 13, y + chk->y + 38, 0x101010);
			Graphics::Line(buffer, bw, bh, x + chk->x + 10, y + chk->y + 34, x + chk->x + 14, y + chk->y + 38, 0x101010);
		}
	}
}

void Controls::ClickCheckBox(unsigned int *checkbox, int x, int y)
{
	// change check status on click
	struct CheckBox *chk = (struct CheckBox *)checkbox;
	if (x >= chk->x + 4 && x <= chk->x + 16 && y >= chk->y + 4 && y <= chk->y + 16)
	{
		if (!chk->lastPressed && chk->active)
		{
			if (!chk->checked) chk->checked = 1;
			else chk->checked = 0;
			chk->lastPressed = 1;
		}
	}
}

void Controls::ReleaseCheckBox(unsigned int *checkbox, int x, int y)
{
	struct CheckBox *chk = (struct CheckBox *)checkbox;
	if (chk->active) chk->lastPressed = 0;
}

// only one button on same chanel can be active at one time
char chanels[0xff];
char btnCount[0xff];

unsigned int *Controls::CreateRadioButton(char *label, int x, int y)
{
	struct RadioButton *rad = (struct RadioButton *)Memory::Alloc(sizeof(struct RadioButton));
	rad->label = label;
	rad->x = x;
	rad->y = y;
	rad->active = 1;
	rad->visible = 1;
	btnCount[0]++;
	return (unsigned int *)rad;
}

void Controls::DestroyRadioButton(unsigned int *radio)
{
	struct RadioButton *rad = (struct RadioButton *)radio;
	btnCount[rad->chanel]--;
	Memory::Free((void *)rad);
}

void Controls::DrawRadioButton(unsigned int *buffer, int bw, int bh, unsigned int *radio, int x, int y)
{
	struct RadioButton *rad = (struct RadioButton *)radio;

	// draw buttons depending on its state but only after updating it
	if (chanels[rad->chanel] && !rad->init)
	{
		rad->marked = 0;
		chanels[rad->chanel]--;
	}
	else if (chanels[rad->chanel] && rad->init) chanels[rad->chanel]--;
	else rad->init = 0;

	if (rad->chanel != rad->lastChanel)
	{
		btnCount[rad->lastChanel]--;
		btnCount[rad->chanel]++;
		rad->lastChanel = rad->chanel;
	}

	int color;
	if (rad->active) color = 0xffffff;
	else color = 0xb0a090;

	if (rad->visible)
	{
		Graphics::FillCircle(buffer, bw, bh, x + rad->x + 13, y + rad->y + 33, 5, color);
		Graphics::Circle(buffer, bw, bh, x + rad->x + 13, y + rad->y + 33, 6, 0x101010);
		Graphics::DrawString(buffer, bw, bh, rad->label, x + rad->x + 24, y + rad->y + 27, 0x101010);
		if (rad->marked) Graphics::FillCircle(buffer, bw, bh, x + rad->x + 13, y + rad->y + 33, 2, 0x101010);
	}
}

void Controls::ClickRadioButton(unsigned int *radio, int x, int y)
{
	struct RadioButton *rad = (struct RadioButton *)radio;

	// just change button state, other buttons state will be changed while redrawing
	if (x >= rad->x + 4 && x <= rad->x + 16 && y >= rad->y + 4 && y <= rad->y + 16)
	{
		rad->marked = 1;
		chanels[rad->chanel] = btnCount[rad->chanel];
		rad->init = 1;
	}
}
