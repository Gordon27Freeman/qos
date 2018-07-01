#include <gui.h>
#include <timer.h>

extern "C" void kmain()
{
	GUI::CreateWindow("Simple Window1", 80, 60, 320, 240);
	GUI::CreateWindow("Simple Window2", 160, 120, 320, 240);
	GUI::CreateWindow("Simple Window3", 240, 180, 320, 240);
	while (1)
	{
		GUI::Update();
		Timer::Wait(1);
	}
}
