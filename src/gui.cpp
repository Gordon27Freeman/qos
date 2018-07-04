#include <gui.h>
#include <graphics.h>
#include <controls.h>
#include <string.h>
#include <mm.h>

#ifdef ARCH_i686
#include <i686/memory.h>
#include <i686/mouse.h>
#include <i686/time.h>
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

#define BLUE 0x00105090
#define ALPH 0xff000000
#define BLUE1 0x001c338a
#define BLUE2 0x002e469f
#define BLUE3 0x003f57b0
#define BLUE4 0x004962bf
#define BLUE5 0x005870cd
#define BLUE6 0x008098f3
#define GRAY1 0x00bfbaaf
#define GRAY2 0x00807d76
#define GRAY3 0x00a7a49e
#define GRAY4 0x008c887f
#define BLACK 0x00101010
#define ALPHA 0xff000000

static unsigned int WindowIcon[16 * 16] =
{
	ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
	ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
	ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
	ALPHA,BLACK,BLUE1,BLUE1,BLUE2,BLUE2,BLUE3,BLUE3,BLUE4,BLUE4,BLUE5,BLUE5,BLUE6,BLUE6,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY2,GRAY2,GRAY2,GRAY2,GRAY2,GRAY1,GRAY3,GRAY3,GRAY3,GRAY3,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY2,GRAY2,GRAY2,GRAY2,GRAY2,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY2,GRAY2,GRAY2,GRAY2,GRAY2,GRAY1,GRAY3,GRAY3,GRAY3,GRAY3,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY3,GRAY3,GRAY3,GRAY3,GRAY3,GRAY3,GRAY3,GRAY3,GRAY3,GRAY3,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY4,GRAY4,GRAY4,GRAY1,GRAY1,GRAY1,GRAY1,GRAY4,GRAY4,GRAY4,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,GRAY1,BLACK,ALPHA,
	ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
	ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
	ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA
};

static unsigned int QIcon[16 * 16] = 
{
	ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,BLUE,BLUE,BLUE,BLUE,BLUE,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,BLUE,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,ALPH,ALPH,
	ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,ALPH,ALPH,
	ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,BLUE,BLUE,BLUE,BLUE,ALPH,ALPH,BLUE,BLUE,ALPH,ALPH,
	ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH,ALPH
};

struct Control
{
	unsigned int *pointer;
	char type;
};

struct Window
{
	char *title;
	unsigned int *buffer;
	int x, y, w, h;
	int px, py, pw, ph;
	char c, s;
	int cx, cy;
	struct Control *controls;
	int controlCount;
	unsigned int uid;
};

struct Window desktop[0xff];
static int windowCount = 0;
static int lastUID = 0;
static int changeActive = 0;
static unsigned int framebuffer[800 * 600];

static void DrawIcon(unsigned int *buffer, int bw, int bh, unsigned int *icon, int x, int y)
{
	for (int cy = y; cy < y + 16; cy++)
		for (int cx = x; cx < x + 16; cx++)
			if (cx > -1 && cy > -1 && cx < bw && cy < bh && (icon[(cx - x) + (cy - y) * 16] >> 24) != 0xff)
				buffer[cx + cy * bw] = icon[(cx - x) + (cy - y) * 16];
}

static void DrawControls(struct Window window)
{
	for (int i = 0; i < window.controlCount; i++)
	{
		switch(window.controls[i].type)
		{
		case CONTROL_BUTTON:
			Controls::DrawButton(window.buffer, window.w, window.h, window.controls[i].pointer);
			break;
		}
	}
}

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
	DrawIcon(window.buffer, window.w, window.h, WindowIcon, 4, 3);
	//DrawControls(window);
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
	Graphics::DrawString(framebuffer, 800, 600, "Menu", 24, 581, 0x101010);
	DrawIcon(framebuffer, 800, 600, QIcon, 4, 580);

	unsigned char hour = 0, minute = 0, second = 0;
	Time::GetTime(&hour, &minute, &second);
	char h[3], m[3];
	itoa(h, 10, hour);
	itoa(m, 10, minute);
	if (!h[1])
	{
		h[1] = h[0];
		h[0] = '0';
	}
	if (!m[1])
	{
		m[1] = m[0];
		m[0] = '0';
	}
	Graphics::DrawString(framebuffer, 800, 600, h, 754, 581, 0x101010);
	Graphics::DrawString(framebuffer, 800, 600, ":", 770, 581, 0x101010);
	Graphics::DrawString(framebuffer, 800, 600, m, 778, 581, 0x101010);
	Graphics::HorizontalLine(framebuffer, 800, 600, 750, 578, 47, 0x907060);
	Graphics::VerticalLine(framebuffer, 800, 600, 750, 578, 19, 0x907060);
	Graphics::HorizontalLine(framebuffer, 800, 600, 751, 597, 47, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 800, 600, 797, 578, 19, 0xf0c0b0);

	int rightBottom, leftTop, color;
	for(int i = 0; i < windowCount; i++)
	{
		if (i == 0 && !desktop[0].s)
		{
			rightBottom = 0xf0c0b0;
			leftTop = 0x907060;
			color = 0xa09080;
		}
		else if (i == 5) break;
		else
		{
			rightBottom = 0x907060;
			leftTop = 0xf0c0b0;
			color = 0xb0a090;
		}
		char title[128];
		if (strlen(desktop[i].title) > 12)
		{
			memcpy(title, desktop[i].title, 13);
			title[9] = '.';
			title[10] = '.';
			title[11] = '.';
			title[12] = 0;
		}
		else strcpy(title, desktop[i].title);
		Graphics::FillRect(framebuffer, 800, 600, 68 + 130 * i, 578, 126, 19, color);
		Graphics::HorizontalLine(framebuffer, 800, 600, 68 + 130 * i, 578, 126, leftTop);
		Graphics::VerticalLine(framebuffer, 800, 600, 68 + 130 * i, 578, 19, leftTop);
		Graphics::HorizontalLine(framebuffer, 800, 600, 68 + 130 * i, 597, 126, rightBottom);
		Graphics::VerticalLine(framebuffer, 800, 600, 193 + 130 * i, 578, 19, rightBottom);
		Graphics::DrawString(framebuffer, 800, 600, title, 90 + 130 * i, 581, 0x101010);
		DrawIcon(framebuffer, 800, 600, WindowIcon, 71 + 130 * i, 580);
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

static void MoveToZero(int n)
{
	struct Window win;
	win.title = desktop[n].title;
	win.x = desktop[n].x;
	win.y = desktop[n].y;
	win.w = desktop[n].w;
	win.h = desktop[n].h;
	win.s = desktop[n].s;
	win.uid = desktop[n].uid;
	win.controls = desktop[n].controls;
	win.controlCount = desktop[n].controlCount;
	win.buffer = desktop[n].buffer;
	for(int i = n; i > 0; i--)
	{
		desktop[i].title = desktop[i - 1].title;
		desktop[i].x = desktop[i - 1].x;
		desktop[i].y = desktop[i - 1].y;
		desktop[i].w = desktop[i - 1].w;
		desktop[i].h = desktop[i - 1].h;
		desktop[i].s = desktop[i - 1].s;
		desktop[i].uid = desktop[i - 1].uid;
		desktop[i].controls = desktop[i - 1].controls;
		desktop[i].controlCount = desktop[i - 1].controlCount;
		desktop[i].buffer = desktop[i - 1].buffer;
	}
	desktop[0].title = win.title;
	desktop[0].x = win.x;
	desktop[0].y = win.y;
	desktop[0].w = win.w;
	desktop[0].h = win.h;
	desktop[0].s = win.s;
	desktop[0].uid = win.uid;
	desktop[0].controls = win.controls;
	desktop[0].controlCount = win.controlCount;
	desktop[0].buffer = win.buffer;
}

static void MoveToEnd()
{
	struct Window win;
	win.title = desktop[0].title;
	win.x = desktop[0].x;
	win.y = desktop[0].y;
	win.w = desktop[0].w;
	win.h = desktop[0].h;
	win.s = desktop[0].s;
	win.uid = desktop[0].uid;
	win.controls = desktop[0].controls;
	win.controlCount = desktop[0].controlCount;
	win.buffer = desktop[0].buffer;
	for(int i = 0; i < windowCount; i++)
	{
		desktop[i].title = desktop[i + 1].title;
		desktop[i].x = desktop[i + 1].x;
		desktop[i].y = desktop[i + 1].y;
		desktop[i].w = desktop[i + 1].w;
		desktop[i].h = desktop[i + 1].h;
		desktop[i].s = desktop[i + 1].s;
		desktop[i].uid = desktop[i + 1].uid;
		desktop[i].controls = desktop[i + 1].controls;
		desktop[i].controlCount = desktop[i + 1].controlCount;
		desktop[i].buffer = desktop[i + 1].buffer;
	}
	desktop[windowCount - 1].title = win.title;
	desktop[windowCount - 1].x = win.x;
	desktop[windowCount - 1].y = win.y;
	desktop[windowCount - 1].w = win.w;
	desktop[windowCount - 1].h = win.h;
	desktop[windowCount - 1].s = win.s;
	desktop[windowCount - 1].uid = win.uid;
	desktop[windowCount - 1].controls = win.controls;
	desktop[windowCount - 1].controlCount = win.controlCount;
	desktop[windowCount - 1].buffer = win.buffer;
}

static void MouseClick()
{
	int MouseY = Mouse::GetY();
	int MouseX = Mouse::GetX();
	int n = (MouseX - 68) / 114;

	if (desktop[0].c == 1)
	{
		desktop[0].x += MouseX - desktop[0].cx;
		desktop[0].y += MouseY - desktop[0].cy;
		desktop[0].cx = MouseX;
		desktop[0].cy = MouseY;
	}
	else if (MouseY > 580 && !changeActive)
	{
		if (MouseX > 68 && MouseX < 638 && n < windowCount)
		{
			if (n && !desktop[n].s)
			{
				MoveToZero(n);
			}
			else if (n && desktop[n].s)
			{
				MoveToZero(n);
				desktop[0].s = 0;
			}
			else if (!n && !desktop[n].s)
			{
				desktop[0].s = 1;
				MoveToEnd();
			}
			else if (!n && desktop[n].s)
			{
				desktop[0].s = 0;
			}
			changeActive = 1;
		}
	}
	else
	{
		n = 0;
		while (n < 0xff)
		{
			if (!desktop[n].s)
			{
				if (MouseX < (desktop[n].x + desktop[n].w) && MouseX > desktop[n].x)
				{
					if (MouseY < (desktop[n].y + desktop[n].h) && MouseY > desktop[n].y)
					{
						MoveToZero(n);
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
							MoveToEnd();
							break;
						}
						else if (MouseY < (desktop[0].y + 22))
						{
							desktop[0].c = 1;
							desktop[0].cx = MouseX;
							desktop[0].cy = MouseY;
							break;
						}

						for (int i = 0; i < desktop[0].controlCount; i++)
						{
							switch(desktop[0].controls[i].type)
							{
							case CONTROL_BUTTON:
								Controls::ClickButton(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
								break;
							}
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
	{
		if (!desktop[i - 1].s)
		{
			//DrawWindow(desktop[i - 1]);
			DrawControls(desktop[i - 1]);
			Graphics::DrawBuffer(framebuffer, desktop[i - 1].buffer, 800, 600, desktop[i - 1].x, desktop[i - 1].y, desktop[i - 1].w, desktop[i - 1].h);
		}
	}
	DrawTaskbar();
	DrawMouse();
	Graphics::DrawFullscreenBuffer(framebuffer);
	if(Mouse::GetLeft()) MouseClick();
	else desktop[0].c = 0;
	if(!Mouse::GetLeft() && changeActive) changeActive = 0;
}

unsigned int GUI::CreateWindow(const char *title, int x, int y, int w, int h)
{
	for(int i = windowCount; i > 0; i--)
	{
		desktop[i].title = desktop[i - 1].title;
		desktop[i].x = desktop[i - 1].x;
		desktop[i].y = desktop[i - 1].y;
		desktop[i].w = desktop[i - 1].w;
		desktop[i].h = desktop[i - 1].h;
		desktop[i].s = desktop[i - 1].s;
		desktop[i].uid = desktop[i - 1].uid;
		desktop[i].controls = desktop[i - 1].controls;
		desktop[i].controlCount = desktop[i - 1].controlCount;
		desktop[i].buffer = desktop[i - 1].buffer;
	}
	desktop[0].title = (char *)Memory::Alloc(strlen(title));
	strcpy(desktop[0].title, title);
	desktop[0].x = x;
	desktop[0].y = y;
	desktop[0].w = w + 4;
	desktop[0].h = h + 24;
	desktop[0].s = 0;
	desktop[0].controls = (struct Control *)Memory::Alloc(sizeof(struct Control) * 1000);
	desktop[0].controlCount = 0;
	desktop[0].buffer = (unsigned int *)Memory::Alloc((w + 4) * (h + 24) * 4);
	desktop[0].uid = lastUID;
	lastUID++;
	DrawWindow(desktop[0]);
	windowCount++;
	return desktop[0].uid;
}

void *GUI::AddControl(unsigned int win, char type)
{
	for(int i = 0; i < windowCount; i++)
	{
		if (win == desktop[i].uid)
		{
			switch(type)
			{
			case CONTROL_BUTTON:
				desktop[i].controls[desktop[i].controlCount].pointer = Controls::CreateButton("Button", 0, 0, 60, 20);
				desktop[i].controls[desktop[i].controlCount].type = type;
				desktop[i].controlCount++;
				DrawWindow(desktop[0]);
				return (void *)desktop[i].controls[desktop[i].controlCount - 1].pointer;
				break;
			}
		}
	}
}

void GUI::DestroyWindow()
{
	for (int i = 0; i < desktop[0].controlCount; i++)
	{
		switch(desktop[0].controls[i].type)
		{
		case CONTROL_BUTTON:
			Controls::DestroyButton(desktop[0].controls[i].pointer);
			break;
		}
	}
	Memory::Free((void *)desktop[0].controls);
	Memory::Free((void *)desktop[0].title);
	Memory::Free((void *)desktop[0].buffer);
	for (int i = 0; i < windowCount; i++)
	{
		desktop[i].title = desktop[i + 1].title;
		desktop[i].x = desktop[i + 1].x;
		desktop[i].y = desktop[i + 1].y;
		desktop[i].w = desktop[i + 1].w;
		desktop[i].h = desktop[i + 1].h;
		desktop[i].s = desktop[i + 1].s;
		desktop[i].uid = desktop[i + 1].uid;
		desktop[i].controls = desktop[i + 1].controls;
		desktop[i].controlCount = desktop[i + 1].controlCount;
		desktop[i].buffer = desktop[i + 1].buffer;
	}
	windowCount--;
}
