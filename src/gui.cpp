#include <gui.h>
#include <graphics.h>
#include <string.h>
#include <mm.h>

#ifdef ARCH_i686
#include <i686/mouse.h>
#endif

using namespace GUI;

struct Window
{
	char *title;
	unsigned int *buffer;
	int x, y, w, h;
	char used = 0;
};

struct Window desktop[0xff];
static int topWindow = 0;
static unsigned int mouseBuffer[11 * 16];

static void DrawWindow(struct Window window)
{
	Graphics::FillRect(window.buffer, window.w, 0, 0, window.w - 1, window.h - 1, 0xb0a090);
	Graphics::HorizontalGradient(window.buffer, window.w, 2, 2, window.w - 4, 19, 0x201080, 0x10c0c0);

	Graphics::HorizontalLine(window.buffer, window.w, 0, window.h - 1, window.w - 1, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.w - 1, 0, window.h - 1, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, 0, 0, window.w - 1, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, 0, 0, window.h - 1, 0xf0c0b0);

	Graphics::FillRect(window.buffer, window.w, window.w - 19, 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(window.buffer, window.w, window.w - 19, 18, 15, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.w - 5, 4, 15, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, window.w - 19, 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, window.w - 19, 4, 15, 0xf0c0b0);
	Graphics::Line(window.buffer, window.w, window.w - 14, 8, window.w - 8, 14, 0xf0c0b0);
	Graphics::Line(window.buffer, window.w, window.w - 8, 8, window.w - 14, 14, 0xf0c0b0);
	Graphics::Line(window.buffer, window.w, window.w - 15, 8, window.w - 9, 14, 0x000000);
	Graphics::Line(window.buffer, window.w, window.w - 9, 8, window.w - 15, 14, 0x000000);

	Graphics::DrawString(window.buffer, window.w, window.title, 6, 4, 0xe0e0e0);

	Graphics::GetBuffer(mouseBuffer, Mouse::GetX(), Mouse::GetY(), 11, 16);
	Graphics::DrawBuffer(window.buffer, window.x, window.y, window.w, window.h);
	Graphics::DrawBuffer(mouseBuffer, Mouse::GetX(), Mouse::GetY(), 11, 16);
}

void GUI::Update()
{
	DrawWindow(desktop[topWindow]);
}

void GUI::CreateWindow(const char *title, int x, int y, int w, int h)
{
	register int n = 0;
	while (n < 0xff)
	{
		if (desktop[n].used == 0)
		{
			desktop[n].title = (char *)Memory::Alloc(strlen(title));
			strcpy(desktop[n].title, title);
			desktop[n].x = x;
			desktop[n].y = y;
			desktop[n].w = w;
			desktop[n].h = h;
			desktop[n].buffer = (unsigned int *)Memory::Alloc(w * h * 4);
			desktop[n].used = 1;
			topWindow = n;
			Update();
			break;
		}
		n++;
	}
}

void GUI::DestroyWindow(const char *title)
{
	register int n = 0;
	while (n < 0xff)
	{
		if (!strcmp(desktop[n].title, title))
		{
			desktop[n].used = 0;
			break;
		}
		n++;
	}
}

void GUI::MousePress(int Button, int MouseX, int MouseY)
{
	register int n = 0;
	while (n < 0xff)
	{
		if (desktop[n].used == 1)
		{
			if (MouseX < (desktop[n].x + desktop[n].w) && MouseX > desktop[n].x)
			{
				if (MouseY < (desktop[n].y + desktop[n].h) && MouseY > desktop[n].y)
				{
					topWindow = n;
					break;
				}
			}
		}
		n++;
	}
}

void GUI::MouseRelease(int Button, int MouseX, int MouseY)
{

}
