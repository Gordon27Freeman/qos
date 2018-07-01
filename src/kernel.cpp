#include <gui.h>

extern "C" void kmain()
{
	GUI::CreateWindow("Simple Window1", 100, 100, 320, 240);
	GUI::CreateWindow("Simple Window2", 200, 200, 400, 200);
	while (1)
	{
		GUI::Update();
		asm volatile("hlt");
	}
}
