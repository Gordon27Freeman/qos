#include <gui.h>
#include <timer.h>
#include <graphics.h>
#include <controls.h>
#include <string.h>
#include <mm.h>

int x = 100, y = 100, count = 0;

void CreateWindowFunc()
{
	char *s = (char *)Memory::Alloc(20);
	char buf[8];
	strcpy(s, "New Window ");
	itoa(buf, 10, count);
	strcpy(s + 11, buf);
	GUI::CreateWindow(s, x, y, 240, 160);
	if (x < 300) x += 50;
	else x = 100;
	if (y < 300) y += 50;
	else y = 100;
	count++;
}

extern "C" void kmain()
{
	window win1 = GUI::CreateWindow("QuantumOS", 80, 60, 200, 100);

	struct Button *btn = (struct Button *)GUI::AddControl(win1, CONTROL_BUTTON);
	btn->label = "Create Window";
	btn->w = 110;
	btn->onClick = CreateWindowFunc;

	while (1) GUI::Update();
}
