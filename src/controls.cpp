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
	return (unsigned int *)lbl;
}

void Controls::DestroyLabel(unsigned int *label)
{
	struct Label *lbl = (struct Label *)label;
	Memory::Free((void *)lbl);
}

void Controls::DrawLabel(unsigned int *buffer, int bw, int bh, unsigned int *label, int x, int y)
{
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
	return (unsigned int *)btn;
}

void Controls::DestroyButton(unsigned int *button)
{
	struct Button *btn = (struct Button *)button;
	Memory::Free((void *)btn);
}

void Controls::DrawButton(unsigned int *buffer, int bw, int bh, unsigned int *button, int x, int y)
{
	struct Button *btn = (struct Button *)button;
	int rightBottom, leftTop, color, textColor;

	if (btn->active) textColor = 0x101010;
	else textColor = 0x806050;

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
	if (x >= btn->x && x <= btn->x + btn->w && y >= btn->y && y <= btn->y + btn->h)
	{
		btn->pressed = 0;
		btn->lastPressed = 0;
	}
}

unsigned int *Controls::CreateTextBox(char *text, int x, int y, int w, int h)
{
	struct TextBox *txt = (struct TextBox *)Memory::Alloc(sizeof(struct TextBox));
	txt->text = (char *)Memory::Alloc(65536);
	strcpy(txt->text, text);
	txt->length = 7;
	txt->x = x;
	txt->y = y;
	txt->w = w;
	txt->h = h;
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
	struct TextBox *txt = (struct TextBox *)textbox;
	if (x >= txt->x && x <= txt->x + txt->w && y >= txt->y && y <= txt->y + txt->h) txt->focus = 1;
	else txt->focus = 0;
}

void Controls::KeyPressTextBox(unsigned int *textbox, char c)
{
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
