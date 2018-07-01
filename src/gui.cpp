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
	char c = 0;
	int cx = 0, cy = 0;
};

struct Window desktop[0xff];
static int windowCount = 0;
static unsigned int mouseBuffer[11 * 16];

static void DrawWindow(struct Window window)
{
	Graphics::FillRect(window.buffer, window.w, 0, 0, window.w - 1, window.h - 1, 0xb0a090);
	Graphics::FillRect(window.buffer, window.w, 2, 2, window.w - 4, 19, 0x104080);
	//Graphics::HorizontalGradient(window.buffer, window.w, 2, 2, window.w - 4, 19, 0x201080, 0x10c0c0);

	Graphics::HorizontalLine(window.buffer, window.w, 0, window.h - 1, window.w, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.w - 1, 0, window.h, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, 0, 0, window.w, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, 0, 0, window.h, 0xf0c0b0);

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
	if (windowCount > 0) DrawWindow(desktop[0]);
}

void GUI::Flush()
{
	unsigned int *framebuffer = Graphics::GetFramebuffer();
	register int cx = 0;
	while (cx < 800 * 600)
	{
		framebuffer[cx] = 0x007b7b;
		cx++;
	}
	for(int i = windowCount; i > 0; i--) DrawWindow(desktop[i - 1]);
}

void GUI::CreateWindow(const char *title, int x, int y, int w, int h)
{
	for(int i = windowCount; i > 0; i--)
	{
		desktop[i].title = desktop[i - 1].title;
		desktop[i].x = desktop[i - 1].x;
		desktop[i].y = desktop[i - 1].y;
		desktop[i].w = desktop[i - 1].w;
		desktop[i].h = desktop[i - 1].h;
		desktop[i].buffer = desktop[i - 1].buffer;
	}
	desktop[0].title = (char *)Memory::Alloc(strlen(title));
	strcpy(desktop[0].title, title);
	desktop[0].x = x;
	desktop[0].y = y;
	desktop[0].w = w;
	desktop[0].h = h;
	desktop[0].buffer = (unsigned int *)Memory::Alloc(w * h * 4);
	DrawWindow(desktop[0]);
	windowCount++;
}

void GUI::DestroyWindow()
{
	for(int i = 0; i < windowCount; i--)
	{
		desktop[i].title = desktop[i + 1].title;
		desktop[i].x = desktop[i + 1].x;
		desktop[i].y = desktop[i + 1].y;
		desktop[i].w = desktop[i + 1].w;
		desktop[i].h = desktop[i + 1].h;
		desktop[i].buffer = desktop[i + 1].buffer;
	}
	Flush();
	windowCount--;
}

void GUI::MousePress(int Button, int MouseX, int MouseY)
{
	register int n = 0;
	while (n < 0xff)
	{
		if (MouseX < (desktop[n].x + desktop[n].w) && MouseX > desktop[n].x)
		{
			if (MouseY < (desktop[n].y + desktop[n].h) && MouseY > desktop[n].y)
			{
				struct Window win;
				win.title = desktop[n].title;
				win.x = desktop[n].x;
				win.y = desktop[n].y;
				win.w = desktop[n].w;
				win.h = desktop[n].h;
				win.buffer = desktop[n].buffer;
				for(int i = n; i > 0; i--)
				{
					desktop[i].title = desktop[i - 1].title;
					desktop[i].x = desktop[i - 1].x;
					desktop[i].y = desktop[i - 1].y;
					desktop[i].w = desktop[i - 1].w;
					desktop[i].h = desktop[i - 1].h;
					desktop[i].buffer = desktop[i - 1].buffer;
				}
				desktop[0].title = win.title;
				desktop[0].x = win.x;
				desktop[0].y = win.y;
				desktop[0].w = win.w;
				desktop[0].h = win.h;
				desktop[0].buffer = win.buffer;

				if (MouseX < (desktop[0].x + desktop[0].w - 4) && MouseX > (desktop[0].x + desktop[0].w - 19))
				{
					if (MouseY < (desktop[0].y + 19) && MouseY > (desktop[0].y + 4))
					{
						DestroyWindow();
						break;
					}
				}

				if (desktop[0].c == 1)
				{
					desktop[0].x += Mouse::GetX() - desktop[0].cx;
					desktop[0].y += Mouse::GetY() - desktop[0].cy;
					desktop[0].cx = Mouse::GetX();
					desktop[0].cy = Mouse::GetY();
				}
				else
				{
					desktop[0].c = 1;
					desktop[0].cx = Mouse::GetX();
					desktop[0].cy = Mouse::GetY();
				}
				break;
			}
		}
		n++;
	}
}

void GUI::MouseRelease(int Button, int MouseX, int MouseY)
{
	desktop[0].c = 0;
}
