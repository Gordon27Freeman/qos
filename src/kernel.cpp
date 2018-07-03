#include <gui.h>
#include <timer.h>
#include <graphics.h>

extern "C" void kmain()
{
	GUI::CreateWindow("Window 1", 100, 100, 320, 240);
	GUI::CreateWindow("Window 2", 150, 150, 320, 240);
	GUI::CreateWindow("Window 3", 200, 200, 320, 240);
	GUI::CreateWindow("Window 4", 250, 250, 320, 240);
	GUI::CreateWindow("Window 5", 300, 300, 320, 240);
	while (1) GUI::Update();
}
