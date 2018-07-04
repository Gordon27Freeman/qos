#include <controls.h>
#include <graphics.h>
#include <string.h>
#include <mm.h>
using namespace Controls;

unsigned int *Controls::CreateButton(char *label, int x, int y, int w, int h)
{
	struct Button *btn = (struct Button *)Memory::Alloc(sizeof(struct Button));
	btn->label = (char *)Memory::Alloc(strlen(label));
	strcpy(btn->label, label);
	btn->x = x;
	btn->y = y;
	btn->w = w;
	btn->h = h;
	return (unsigned int *)btn;
}

void Controls::DestroyButton(unsigned int *button)
{
	struct Button *btn = (struct Button *)button;
	Memory::Free((void *)btn->label);
	Memory::Free((void *)btn);
}

void Controls::DrawButton(unsigned int *buffer, int bw, int bh, unsigned int *button)
{
	struct Button *btn = (struct Button *)button;
	int rightBottom, leftTop, color;
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
	Graphics::FillRect(buffer, bw, bh, btn->x + 4, btn->y + 24, btn->w, btn->h, color);
	Graphics::HorizontalLine(buffer, bw, bh, btn->x + 4, btn->y + 24, btn->w, leftTop);
	Graphics::VerticalLine(buffer, bw, bh, btn->x + 4, btn->y + 24, btn->h, leftTop);
	Graphics::HorizontalLine(buffer, bw, bh, btn->x + 4, btn->y + btn->h + 24, btn->w + 1, rightBottom);
	Graphics::VerticalLine(buffer, bw, bh, btn->x + btn->w + 4, btn->y + 24, btn->h, rightBottom);
	Graphics::DrawString(buffer, bw, bh, btn->label, btn->x + 8, btn->y + 27, 0x101010);
	btn->pressed = 0;
}

void Controls::ClickButton(unsigned int *button, int x, int y)
{
	struct Button *btn = (struct Button *)button;
	if (x >= btn->x && x <= btn->x + btn->w && y >= btn->y && y <= btn->y + btn->h)
	{
		btn->pressed = 1;
	}
}
