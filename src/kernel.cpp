#include <gui.h>
#include <timer.h>
#include <graphics.h>
#include <controls.h>

extern "C" void kmain()
{
	GUI::CreateWindow("QuantumOS", 80, 60, 640, 480);
	GUI::CreateWindow("Simple Window 1", 150, 100, 320, 240);
	GUI::CreateWindow("Simple Window 2", 200, 150, 320, 240);

	struct Button *btn = (struct Button *)GUI::AddControl("QuantumOS", CONTROL_BUTTON);
	btn->label = "Hello, World!";
	btn->w = 110;
	btn->x = 265;
	btn->y = 230;

	struct Button *btn1 = (struct Button *)GUI::AddControl("Simple Window 1", CONTROL_BUTTON);
	btn1->label = "BTN1\0";

	struct Button *btn2 = (struct Button *)GUI::AddControl("Simple Window 2", CONTROL_BUTTON);
	btn2->label = "BTN2\0";

	while (1) GUI::Update();
}
