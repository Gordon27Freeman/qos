#include <gui.h>
#include <graphics.h>
#include <string.h>
#include <mm.h>

#ifdef ARCH_i686
#include <i686/mouse.h>
#endif

using namespace GUI;

static unsigned int Cursor[11 * 19] =
{
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 1, 0, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 1, 0, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 1, 0, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 1, 0, 0,
	1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0,
	1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
	1, 2, 2, 1, 2, 2, 1, 0, 0, 0, 0,
	1, 2, 1, 0, 1, 2, 2, 1, 0, 0, 0,
	1, 1, 0, 0, 1, 2, 2, 1, 0, 0, 0,
	1, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 1, 2, 2, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 1, 2, 2, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0
};

static unsigned int Icon[16 * 16] = 
{
	
};

struct Window
{
	char *title;
	unsigned int *buffer;
	int x, y, w, h;
	int px, py, pw, ph;
	char c, s;
	int cx, cy;
};

struct Window desktop[0xff];
static int windowCount = 0;
static unsigned int framebuffer[800 * 600];

static void DrawWindow(struct Window window)
{
	Graphics::FillRect(window.buffer, window.w, window.h, 0, 0, window.w - 1, window.h - 1, 0xb0a090);
	Graphics::HorizontalGradient(window.buffer, window.w, window.h, 2, 2, window.w - 4, 19, 0x201080, 0x10c0c0);

	Graphics::HorizontalLine(window.buffer, window.w, window.h, 0, window.h - 1, window.w, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 1, 0, window.h, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, 0, 0, window.w, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, window.h, 0, 0, window.h, 0xf0c0b0);

	Graphics::FillRect(window.buffer, window.w, window.h, window.w - 55, 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 55, 18, 15, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 41, 4, 15, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 55, 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 55, 4, 15, 0xf0c0b0);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 51, 14, 8, 0xf0c0b0);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 52, 15, 8, 0x404040);

	Graphics::FillRect(window.buffer, window.w, window.h, window.w - 37, 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 37, 18, 15, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 23, 4, 15, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 37, 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 37, 4, 15, 0xf0c0b0);
	Graphics::Rect(window.buffer, window.w, window.h, window.w - 33, 6, 8, 8, 0xf0c0b0);
	Graphics::Rect(window.buffer, window.w, window.h, window.w - 34, 7, 8, 8, 0x404040);

	Graphics::FillRect(window.buffer, window.w, window.h, window.w - 19, 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 19, 18, 15, 0x907060);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 5, 4, 15, 0x907060);
	Graphics::HorizontalLine(window.buffer, window.w, window.h, window.w - 19, 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(window.buffer, window.w, window.h, window.w - 19, 4, 15, 0xf0c0b0);
	Graphics::Line(window.buffer, window.w, window.h, window.w - 14, 8, window.w - 8, 14, 0xf0c0b0);
	Graphics::Line(window.buffer, window.w, window.h, window.w - 8, 8, window.w - 14, 14, 0xf0c0b0);
	Graphics::Line(window.buffer, window.w, window.h, window.w - 15, 8, window.w - 9, 14, 0x000000);
	Graphics::Line(window.buffer, window.w, window.h, window.w - 9, 8, window.w - 15, 14, 0x000000);

	Graphics::DrawString(window.buffer, window.w, window.h, window.title, 24, 4, 0xe0e0e0);
}

static void DrawTaskbar()
{
	Graphics::FillRect(framebuffer, 800, 600, 0, 576, 800, 24, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 800, 600, 0, 575, 800, 0xf0c0b0);

	Graphics::FillRect(framebuffer, 800, 600, 2, 578, 62, 19, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 800, 600, 2, 578, 62, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 800, 600, 2, 578, 19, 0xf0c0b0);
	Graphics::HorizontalLine(framebuffer, 800, 600, 2, 597, 62, 0x907060);
	Graphics::VerticalLine(framebuffer, 800, 600, 63, 578, 19, 0x907060);
	Graphics::DrawString(framebuffer, 800, 600, "Menu", 24, 580, 0x101010);

	for(int i = 0; i < windowCount; i++)
	{
		if (i == 5) break;
		Graphics::FillRect(framebuffer, 800, 600, 68 + 114 * i, 578, 110, 19, 0xb0a090);
		Graphics::HorizontalLine(framebuffer, 800, 600, 68 + 114 * i, 578, 110, 0xf0c0b0);
		Graphics::VerticalLine(framebuffer, 800, 600, 68 + 114 * i, 578, 19, 0xf0c0b0);
		Graphics::HorizontalLine(framebuffer, 800, 600, 68 + 114 * i, 597, 110, 0x907060);
		Graphics::VerticalLine(framebuffer, 800, 600, 177 + 114 * i, 578, 19, 0x907060);
		Graphics::DrawString(framebuffer, 800, 600, desktop[i].title, 90 + 114 * i, 580, 0x101010);
	}
}

static void DrawMouse()
{
	int MouseY = Mouse::GetY();
	int MouseX = Mouse::GetX();
	int cy = MouseY, cx = MouseX, ly = MouseY + 19, lx = MouseX + 11;
	while (cy < ly)
	{
		while (cx < lx)
		{
			switch (Cursor[(cx - MouseX) + (cy - MouseY) * 11])
			{
			case 1:
				if (cx < 800 && cy < 600 && cx > -1 && cy > -1) framebuffer[cx + cy * 800] = 0;
				break;
			case 2:
				if (cx < 800 && cy < 600 && cx > -1 && cy > -1) framebuffer[cx + cy * 800] = 0xFFFFFF;
				break;
			}
			cx++;
		}
		cx = MouseX;
		cy++;
	}
}

static void MouseClick()
{
	int MouseY = Mouse::GetY();
	int MouseX = Mouse::GetX();
	int n = 0;

	if (desktop[0].c == 1)
	{
		desktop[0].x += MouseX - desktop[0].cx;
		desktop[0].y += MouseY - desktop[0].cy;
		desktop[0].cx = MouseX;
		desktop[0].cy = MouseY;
	}
	else
	{
		while (n < 0xff)
		{
			if (!desktop[n].s)
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
						win.s = desktop[n].s;
						win.buffer = desktop[n].buffer;
						for(int i = n; i > 0; i--)
						{
							desktop[i].title = desktop[i - 1].title;
							desktop[i].x = desktop[i - 1].x;
							desktop[i].y = desktop[i - 1].y;
							desktop[i].w = desktop[i - 1].w;
							desktop[i].h = desktop[i - 1].h;
							desktop[i].s = desktop[i - 1].s;
							desktop[i].buffer = desktop[i - 1].buffer;
						}
						desktop[0].title = win.title;
						desktop[0].x = win.x;
						desktop[0].y = win.y;
						desktop[0].w = win.w;
						desktop[0].h = win.h;
						desktop[0].s = win.s;
						desktop[0].buffer = win.buffer;

						if (MouseX < (desktop[0].x + desktop[0].w - 4) && MouseX > (desktop[0].x + desktop[0].w - 19) && MouseY < (desktop[0].y + 20))
						{
							if (!desktop[0].c) DestroyWindow();
							break;
						}
						else if (MouseX < (desktop[0].x + desktop[0].w - 21) && MouseX > (desktop[0].x + desktop[0].w - 37) && MouseY < (desktop[0].y + 20))
						{
							if (desktop[0].w != 800 && desktop[0].h != 575)
							{
								desktop[0].pw = desktop[0].w;
								desktop[0].ph = desktop[0].h;
								desktop[0].px = desktop[0].x;
								desktop[0].py = desktop[0].y;
								desktop[0].w = 800;
								desktop[0].h = 575;
								desktop[0].x = 0;
								desktop[0].y = 0;
								desktop[0].buffer = (unsigned int *)Memory::Realloc(desktop[0].buffer, 800 * 575 * 4);
							}
							else
							{
								desktop[0].w = desktop[0].pw;
								desktop[0].h = desktop[0].ph;
								desktop[0].x = desktop[0].px;
								desktop[0].y = desktop[0].py;
								desktop[0].buffer = (unsigned int *)Memory::Realloc(desktop[0].buffer, desktop[0].w * desktop[0].h * 4);
							}
							DrawWindow(desktop[0]);
							break;
						}
						else if (MouseX < (desktop[0].x + desktop[0].w - 38) && MouseX > (desktop[0].x + desktop[0].w - 55) && MouseY < (desktop[0].y + 20))
						{
							desktop[0].s = 1;
							break;
						}
						else if (MouseY < (desktop[0].y + 22))
						{
							
							desktop[0].c = 1;
							desktop[0].cx = MouseX;
							desktop[0].cy = MouseY;
							break;
						}
						break;
					}
				}
			}
			n++;
		}
	}
}

void GUI::Update()
{
	int cx = 0;
	while (cx < 800 * 600)
	{
		framebuffer[cx] = 0x108080;
		cx++;
	}
	for (int i = windowCount; i > 0; i--)
		if (!desktop[i - 1].s) Graphics::DrawBuffer(framebuffer, desktop[i - 1].buffer, 800, 600, desktop[i - 1].x, desktop[i - 1].y, desktop[i - 1].w, desktop[i - 1].h);
	DrawTaskbar();
	DrawMouse();
	Graphics::DrawFullscreenBuffer(framebuffer);
	if(Mouse::GetLeft()) MouseClick();
	else desktop[0].c = 0;
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
		desktop[i].s = desktop[i - 1].s;
		desktop[i].buffer = desktop[i - 1].buffer;
	}
	desktop[0].title = (char *)Memory::Alloc(strlen(title));
	strcpy(desktop[0].title, title);
	desktop[0].x = x;
	desktop[0].y = y;
	desktop[0].w = w;
	desktop[0].h = h;
	desktop[0].s = 0;
	desktop[0].buffer = (unsigned int *)Memory::Alloc(w * h * 4);
	DrawWindow(desktop[0]);
	windowCount++;
}

void GUI::DestroyWindow()
{
	Memory::Free((void *)&desktop[0].title[0]);
	Memory::Free((void *)&desktop[0].buffer[0]);
	for(int i = 0; i < windowCount; i++)
	{
		desktop[i].title = desktop[i + 1].title;
		desktop[i].x = desktop[i + 1].x;
		desktop[i].y = desktop[i + 1].y;
		desktop[i].w = desktop[i + 1].w;
		desktop[i].h = desktop[i + 1].h;
		desktop[i].s = desktop[i + 1].s;
		desktop[i].buffer = desktop[i + 1].buffer;
	}
	windowCount--;
}
