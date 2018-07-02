#include <gui.h>
#include <timer.h>
#include <graphics.h>

extern "C" void kmain()
{
	GUI::CreateWindow("Window 1", 80, 60, 320, 240);
	GUI::CreateWindow("Window 2", 160, 120, 320, 240);
	GUI::CreateWindow("Window 3", 240, 180, 320, 240);
	GUI::CreateWindow("Window 4", 320, 240, 320, 240);
	GUI::CreateWindow("Window 5", 400, 300, 320, 240);
	while (1) GUI::Update();
}
