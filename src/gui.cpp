#include <gui.h>
#include <graphics.h>
#include <controls.h>
#include <string.h>
#include <mm.h>

#ifdef ARCH_i686
#include <i686/keyboard.h>
#include <i686/memory.h>
#include <i686/mouse.h>
#include <i686/time.h>
#endif

using namespace GUI;

// cursor bitmap nothing special
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

// define some colors to create some basic icons
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

// default window icon
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

// icon for start menu
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
	const char *title;
	unsigned int *buffer;
	int x, y, w, h;
	int px, py, pw, ph;
	char c, r, s;
	int cx, cy, rx, ry;
	char resizable;
	struct Control *controls;
	int controlCount;
	unsigned int uid;
};

// we dont need more than 45 windows
// it hurts to display all of them
// also window list cant display more ¯\_(ツ)_/¯
struct Window desktop[45];
static int windowCount = 0;
static int lastUID = 1;
static int changeActive = 0;
static int clickHold = 0;
static int listShown = 0;
static unsigned int framebuffer[1024 * 768];

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
		case CONTROL_LABEL:
			Controls::DrawLabel(framebuffer, 1024, 768, window.controls[i].pointer, window.x, window.y);
			break;
		case CONTROL_BUTTON:
			Controls::DrawButton(framebuffer, 1024, 768, window.controls[i].pointer, window.x, window.y);
			break;
		case CONTROL_TEXTBOX:
			Controls::DrawTextBox(framebuffer, 1024, 768, window.controls[i].pointer, window.x, window.y);
			break;
		case CONTROL_CHECKBOX:
			Controls::DrawCheckBox(framebuffer, 1024, 768, window.controls[i].pointer, window.x, window.y);
			break;
		case CONTROL_RADIO:
			Controls::DrawRadioButton(framebuffer, 1024, 768, window.controls[i].pointer, window.x, window.y);
			break;
		}
	}
}

char title[128];

static void DrawWindow(struct Window window, char n)
{
	int startColor = 0x404040, endColor = 0xa0a0a0;
	if (!n)
	{
		startColor = 0x201080;
		endColor = 0x10c0c0;
	}

	// forgot what each function draws but it doesnt matter at all
	Graphics::FillRect(framebuffer, 1024, 768, window.x + 0, window.y + 0, window.w - 1, window.h - 1, 0xb0a090);
	Graphics::HorizontalGradient(framebuffer, 1024, 768, window.x + 2, window.y + 2, window.w - 4, 19, startColor, endColor);

	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + 0, window.y + window.h - 1, window.w, 0x907060);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 1, window.y + 0, window.h, 0x907060);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + 0, window.y + 0, window.w, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + 0, window.y + 0, window.h, 0xf0c0b0);

	Graphics::FillRect(framebuffer, 1024, 768, window.x + window.w - 55, window.y + 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 55, window.y + 18, 15, 0x907060);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 41, window.y + 4, 15, 0x907060);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 55, window.y + 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 55, window.y + 4, 15, 0xf0c0b0);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 51, window.y + 14, 8, 0xf0c0b0);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 52, window.y + 15, 8, 0x404040);

	Graphics::FillRect(framebuffer, 1024, 768, window.x + window.w - 37, window.y + 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 37, window.y + 18, 15, 0x907060);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 23, window.y + 4, 15, 0x907060);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 37, window.y + 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 37, window.y + 4, 15, 0xf0c0b0);
	Graphics::Rect(framebuffer, 1024, 768, window.x + window.w - 33, window.y + 6, 8, 8, 0xf0c0b0);
	Graphics::Rect(framebuffer, 1024, 768, window.x + window.w - 34, window.y + 7, 8, 8, 0x404040);

	Graphics::FillRect(framebuffer, 1024, 768, window.x + window.w - 19, window.y + 4, 15, 15, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 19, window.y + 18, 15, 0x907060);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 5, window.y + 4, 15, 0x907060);
	Graphics::HorizontalLine(framebuffer, 1024, 768, window.x + window.w - 19, window.y + 4, 15, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 1024, 768, window.x + window.w - 19, window.y + 4, 15, 0xf0c0b0);

	Graphics::Line(framebuffer, 1024, 768, window.x + window.w - 14, window.y + 8, window.x + window.w - 8, window.y + 14, 0xf0c0b0);
	Graphics::Line(framebuffer, 1024, 768, window.x + window.w - 8, window.y + 8, window.x + window.w - 14, window.y + 14, 0xf0c0b0);
	Graphics::Line(framebuffer, 1024, 768, window.x + window.w - 15, window.y + 8, window.x + window.w - 9, window.y + 14, 0x000000);
	Graphics::Line(framebuffer, 1024, 768, window.x + window.w - 9, window.y + 8, window.x + window.w - 15, window.y + 14, 0x000000);

	// cut window title to fit its size
	strcpy(title, window.title);
	if (strlen(window.title) > ((window.w - 80) / 8))
	{
		title[((window.w - 80) / 8) - 4] = '.';
		title[((window.w - 80) / 8) - 3] = '.';
		title[((window.w - 80) / 8) - 2] = '.';
		title[((window.w - 80) / 8) - 1] = 0;
	}

	Graphics::DrawString(framebuffer, 1024, 768, title, window.x + 24, window.y + 4, 0xe0e0e0);
	DrawIcon(framebuffer, 1024, 768, WindowIcon, window.x + 4, window.y + 3);
	DrawControls(window);
}

static void DrawList()
{
	for(int i = 6; i < windowCount; i++)
	{
		if (i > 45) break;
		int rightBottom = 0x907060;
		int leftTop = 0xf0c0b0;
		int color = 0xb0a090;

		// cut window title to fit button size in window list
		strcpy(title, desktop[i].title);
		if (strlen(desktop[i].title) > 15)
		{
			
			title[12] = '.';
			title[13] = '.';
			title[14] = '.';
			title[15] = 0;
		}

		Graphics::FillRect(framebuffer, 1024, 768, 875, 743 - (i - 5) * 19, 146, 19, color);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 875, 743 - (i - 5) * 19, 146, leftTop);
		Graphics::VerticalLine(framebuffer, 1024, 768, 875, 743 - (i - 5) * 19, 19, leftTop);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 875, 762 - (i - 5) * 19, 146, rightBottom);
		Graphics::VerticalLine(framebuffer, 1024, 768, 1020, 743 - (i - 5) * 19, 19, rightBottom);
		Graphics::DrawString(framebuffer, 1024, 768, title, 897, 746 - (i - 5) * 19, 0x101010);
		DrawIcon(framebuffer, 1024, 768, WindowIcon, 878, 745 - (i - 5) * 19);
	}
}

static void DrawTaskbar()
{
	// this draws start menu button
	Graphics::FillRect(framebuffer, 1024, 768, 0, 744, 1024, 24, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 1024, 768, 0, 745, 1024, 0xf0c0b0);

	Graphics::FillRect(framebuffer, 1024, 768, 2, 747, 62, 19, 0xb0a090);
	Graphics::HorizontalLine(framebuffer, 1024, 768, 2, 747, 62, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 1024, 768, 2, 747, 19, 0xf0c0b0);
	Graphics::HorizontalLine(framebuffer, 1024, 768, 2, 766, 62, 0x907060);
	Graphics::VerticalLine(framebuffer, 1024, 768, 63, 747, 19, 0x907060);
	Graphics::DrawString(framebuffer, 1024, 768, "Menu", 24, 750, 0x101010);
	DrawIcon(framebuffer, 1024, 768, QIcon, 4, 749);

	// this draws time in the right bottom place of screen
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
	Graphics::DrawString(framebuffer, 1024, 768, h, 978, 750, 0x101010);
	Graphics::DrawString(framebuffer, 1024, 768, ":", 994, 750, 0x101010);
	Graphics::DrawString(framebuffer, 1024, 768, m, 1002, 750, 0x101010);
	Graphics::HorizontalLine(framebuffer, 1024, 768, 974, 747, 47, 0x907060);
	Graphics::VerticalLine(framebuffer, 1024, 768, 974, 747, 19, 0x907060);
	Graphics::HorizontalLine(framebuffer, 1024, 768, 975, 765, 47, 0xf0c0b0);
	Graphics::VerticalLine(framebuffer, 1024, 768, 1021, 747, 19, 0xf0c0b0);

	// 6 window buttons fits into entire taskbar
	// if more draw button to open window list
	if (windowCount > 6)
	{
		Graphics::HorizontalLine(framebuffer, 1024, 768, 961, 747, 10, 0xf0c0b0);
		Graphics::VerticalLine(framebuffer, 1024, 768, 961, 747, 19, 0xf0c0b0);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 961, 766, 11, 0x907060);
		Graphics::VerticalLine(framebuffer, 1024, 768, 971, 747, 19, 0x907060);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 966, 755, 1, 0x101010);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 965, 756, 3, 0x101010);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 964, 757, 5, 0x101010);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 963, 758, 7, 0x101010);
	}

	int rightBottom, leftTop, color;
	for(int i = 0; i < windowCount; i++)
	{
		if (i == 0 && !desktop[0].s)
		{
			rightBottom = 0xf0c0b0;
			leftTop = 0x907060;
			color = 0xa09080;
		}
		else if (i == 6) break;
		else
		{
			rightBottom = 0x907060;
			leftTop = 0xf0c0b0;
			color = 0xb0a090;
		}

		strcpy(title, desktop[i].title);
		if (strlen(desktop[i].title) > 15)
		{
			title[12] = '.';
			title[13] = '.';
			title[14] = '.';
			title[15] = 0;
		}

		// this draws window buttons
		Graphics::FillRect(framebuffer, 1024, 768, 68 + 149 * i, 747, 146, 19, color);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 68 + 149 * i, 747, 146, leftTop);
		Graphics::VerticalLine(framebuffer, 1024, 768, 68 + 149 * i, 747, 19, leftTop);
		Graphics::HorizontalLine(framebuffer, 1024, 768, 68 + 149 * i, 766, 147, rightBottom);
		Graphics::VerticalLine(framebuffer, 1024, 768, 214 + 149 * i, 747, 19, rightBottom);
		Graphics::DrawString(framebuffer, 1024, 768, title, 90 + 149 * i, 750, 0x101010);
		DrawIcon(framebuffer, 1024, 768, WindowIcon, 71 + 149 * i, 749);
	}

	if (listShown) DrawList();
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
				if (cx < 1024 && cy < 768 && cx > -1 && cy > -1) framebuffer[cx + cy * 1024] = 0;
				break;
			case 2:
				if (cx < 1024 && cy < 768 && cx > -1 && cy > -1) framebuffer[cx + cy * 1024] = 0xFFFFFF;
				break;
			}
			cx++;
		}
		cx = MouseX;
		cy++;
	}
}

// we need to move windows though window array so use this to simplify code
static void MoveWindow(Window *dest, Window *src)
{
	dest->title = src->title;
	dest->x = src->x;
	dest->y = src->y;
	dest->w = src->w;
	dest->h = src->h;
	dest->s = src->s;
	dest->px = src->px;
	dest->py = src->py;
	dest->pw = src->pw;
	dest->ph = src->ph;
	dest->resizable = src->resizable;
	dest->uid = src->uid;
	dest->controls = src->controls;
	dest->controlCount = src->controlCount;
}

// more useful functions
static void MoveToZero(int n)
{
	struct Window win;
	MoveWindow(&win, &desktop[n]);
	for(int i = n; i > 0; i--)
		MoveWindow(&desktop[i], &desktop[i - 1]);
	MoveWindow(&desktop[0], &win);
}

static void MoveToEnd()
{
	struct Window win;
	MoveWindow(&win, &desktop[0]);
	for(int i = 0; i < windowCount; i++)
		MoveWindow(&desktop[i], &desktop[i + 1]);
	MoveWindow(&desktop[windowCount - 1], &win);
}

// this is where the most of gui work comes
static void MouseClick()
{
	int MouseY = Mouse::GetY();
	int MouseX = Mouse::GetX();
	int n = (MouseX - 67) / 149;
	int winMax;
	// set number of window buttons on taskbar
	if (windowCount < 7) winMax = windowCount;
	else winMax = 6;

	// if opened window list check if list buttons was pressed
	if (listShown && !changeActive)
	{
		if (MouseX > 875 && MouseX < 1020 && MouseY > (743 - (windowCount - 6) * 19) && MouseY < 743)
		{
			n = (743 - MouseY) / 19 + 6;
			MoveToZero(n);
			if (desktop[0].s) desktop[0].s = 0;
			changeActive = 1;
		}
	}

	// if moving window
	if (desktop[0].c == 1)
	{
		desktop[0].x += MouseX - desktop[0].cx;
		desktop[0].y += MouseY - desktop[0].cy;
		desktop[0].cx = MouseX;
		desktop[0].cy = MouseY;
	}
	// if resizing window
	if (desktop[0].r == 1)
	{
		desktop[0].w += MouseX - desktop[0].rx;
		desktop[0].h += MouseY - desktop[0].ry;
		desktop[0].rx = MouseX;
		desktop[0].ry = MouseY;
		if (desktop[0].w < 100) desktop[0].w = 100;
		if (desktop[0].h < 24) desktop[0].h = 24;
	}
	// clicks on taskbar
	else if (MouseY > 748 && !changeActive)
	{
		// if windows button was pressed move it on top of desktop
		if (MouseX > 68 && n < winMax)
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
		// open window list
		else if (MouseX > 961 && MouseX < 972)
		{
			if (!changeActive)
			{
				if (!listShown) listShown = 1;
				else listShown = 0;
			}
			changeActive = 1;
		}
	}
	else // clicking on desktop or windows
	{
		n = 0;
		// check every window
		while (n < 45)
		{
			// but only if its shown
			if (!desktop[n].s)
			{
				if (MouseX < (desktop[n].x + desktop[n].w) && MouseX > desktop[n].x)
				{
					if (MouseY < (desktop[n].y + desktop[n].h) && MouseY > desktop[n].y)
					{
						// so we clicked shown window!
						MoveToZero(n);
						// close window if clicked close button .-.
						if (MouseX < (desktop[0].x + desktop[0].w - 4) && MouseX > (desktop[0].x + desktop[0].w - 19) && MouseY < (desktop[0].y + 20))
						{
							if (!desktop[0].c)
							{
								DestroyWindow();
								clickHold = 1;
							}
							break;
						}
						// maximize or normalize(?) window if clicked this button oaoaoe mmm
						else if (MouseX < (desktop[0].x + desktop[0].w - 21) && MouseX > (desktop[0].x + desktop[0].w - 37) && MouseY < (desktop[0].y + 20))
						{
							if (desktop[0].resizable)
							{
								if (desktop[0].w != 1024 && desktop[0].h != 743)
								{
									desktop[0].pw = desktop[0].w;
									desktop[0].ph = desktop[0].h;
									desktop[0].px = desktop[0].x;
									desktop[0].py = desktop[0].y;
									desktop[0].w = 1024;
									desktop[0].h = 743;
									desktop[0].x = 0;
									desktop[0].y = 0;
								}
								else
								{
									desktop[0].w = desktop[0].pw;
									desktop[0].h = desktop[0].ph;
									desktop[0].x = desktop[0].px;
									desktop[0].y = desktop[0].py;
								}
								clickHold = 1;
							}
							break;
						}
						// minimize button clicked
						else if (MouseX < (desktop[0].x + desktop[0].w - 38) && MouseX > (desktop[0].x + desktop[0].w - 55) && MouseY < (desktop[0].y + 20))
						{
							desktop[0].s = 1;
							MoveToEnd();
							clickHold = 1;
							break;
						}
						// if clicked on top of window start dragging it
						else if (MouseY < (desktop[0].y + 22))
						{
							desktop[0].c = 1;
							desktop[0].cx = MouseX;
							desktop[0].cy = MouseY;
							break;
						}
						// if clicked in the right bottom corner start resizing it
						else if (MouseX > (desktop[0].x + desktop[0].w - 5) && MouseX < (desktop[0].x + desktop[0].w) && MouseY > (desktop[0].y + desktop[0].h - 5) && MouseY < (desktop[0].y + desktop[0].h))
						{
							if (desktop[0].resizable)
							{
								desktop[0].r = 1;
								desktop[0].rx = MouseX;
								desktop[0].ry = MouseY;
							}
							break;
						}

						// in any other case process click event for every window control
						for (int i = 0; i < desktop[0].controlCount; i++)
						{
							switch(desktop[0].controls[i].type)
							{
							case CONTROL_BUTTON:
								Controls::ClickButton(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
								break;
							case CONTROL_TEXTBOX:
								Controls::ClickTextBox(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
								break;
							case CONTROL_CHECKBOX:
								Controls::ClickCheckBox(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
								break;
							case CONTROL_RADIO:
								Controls::ClickRadioButton(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
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
	// stylish background
	while (cx < 1024 * 768)
	{
		framebuffer[cx] = 0x108080;
		cx++;
	}

	// first draw shown windows in right order
	for (int i = windowCount; i > 0; i--)
		if (!desktop[i - 1].s) DrawWindow(desktop[i - 1], i - 1);

	// then draw taskbar and mouse on top of all this shit
	DrawTaskbar();
	DrawMouse();
	// redraw screen
	Graphics::DrawFullscreenBuffer(framebuffer);

	// process keyboard events for textboxes
	char c = Keyboard::GetChar();
	if(c)
	{
		for (int i = 0; i < desktop[0].controlCount; i++)
		{
			switch(desktop[0].controls[i].type)
			{
			case CONTROL_TEXTBOX:
				Controls::KeyPressTextBox(desktop[0].controls[i].pointer, c);
				break;
			}
		}
	}

	// update mouse click state to avoid infinitely fast clicking
	if(Mouse::GetLeft())
	{
		if (!clickHold) MouseClick();
	}
	else // need also release event
	{
		clickHold = 0;
		int MouseY = Mouse::GetY();
		int MouseX = Mouse::GetX();
		desktop[0].c = 0;
		desktop[0].r = 0;
		for (int i = 0; i < desktop[0].controlCount; i++)
		{
			switch(desktop[0].controls[i].type)
			{
			case CONTROL_BUTTON:
				Controls::ReleaseButton(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
				break;
			case CONTROL_CHECKBOX:
				Controls::ReleaseCheckBox(desktop[0].controls[i].pointer, MouseX - desktop[0].x - 4, MouseY - desktop[0].y - 24);
				break;
			}
		}
	}
	// need this to process next click only if mouse button was released
	if(!Mouse::GetLeft() && changeActive) changeActive = 0;
}

unsigned int GUI::CreateWindow(const char *title, int x, int y, int w, int h, int resizable)
{
	if (windowCount == 45) return 0;
	// move new window on top of window array
	for(int i = windowCount; i > 0; i--)
		MoveWindow(&desktop[i], &desktop[i - 1]);

	desktop[0].title = title;
	desktop[0].x = x;
	desktop[0].y = y;
	desktop[0].w = w + 4;
	desktop[0].h = h + 24;
	if (desktop[0].w < 100) desktop[0].w = 100;
	if (desktop[0].h < 24) desktop[0].h = 24;
	desktop[0].s = 0;
	desktop[0].resizable = resizable;
	// allocate some memory to store window controls there
	desktop[0].controls = (struct Control *)Memory::Alloc(sizeof(struct Control) * 1024);
	desktop[0].controlCount = 0;
	desktop[0].uid = lastUID;
	lastUID++;
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
			case CONTROL_LABEL:
				desktop[i].controls[desktop[i].controlCount].pointer = Controls::CreateLabel("Label", 0, 0);
				break;
			case CONTROL_BUTTON:
				desktop[i].controls[desktop[i].controlCount].pointer = Controls::CreateButton("Button", 0, 0, 64, 20);
				break;
			case CONTROL_TEXTBOX:
				desktop[i].controls[desktop[i].controlCount].pointer = Controls::CreateTextBox("TextBox", 0, 0, 64, 20);
				break;
			case CONTROL_CHECKBOX:
				desktop[i].controls[desktop[i].controlCount].pointer = Controls::CreateCheckBox("CheckBox", 0, 0);
				break;
			case CONTROL_RADIO:
				desktop[i].controls[desktop[i].controlCount].pointer = Controls::CreateRadioButton("RadioButton", 0, 0);
				break;
			}
			desktop[i].controls[desktop[i].controlCount].type = type;
			desktop[i].controlCount++;
			return (void *)desktop[i].controls[desktop[i].controlCount - 1].pointer;
		}
	}
}

void GUI::DestroyWindow()
{
	for (int i = 0; i < desktop[0].controlCount; i++)
	{
		switch(desktop[0].controls[i].type)
		{
		case CONTROL_LABEL:
			Controls::DestroyLabel(desktop[0].controls[i].pointer);
			break;
		case CONTROL_BUTTON:
			Controls::DestroyButton(desktop[0].controls[i].pointer);
			break;
		case CONTROL_TEXTBOX:
			Controls::DestroyTextBox(desktop[0].controls[i].pointer);
			break;
		case CONTROL_CHECKBOX:
			Controls::DestroyCheckBox(desktop[0].controls[i].pointer);
			break;
		case CONTROL_RADIO:
			Controls::DestroyRadioButton(desktop[0].controls[i].pointer);
			break;
		}
	}
	Memory::Free((void *)desktop[0].controls);
	Memory::Free((void *)desktop[0].title);
	for (int i = 0; i < windowCount; i++)
		MoveWindow(&desktop[i], &desktop[i + 1]);
	windowCount--;
}
