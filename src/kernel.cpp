#include <gui.h>
#include <controls.h>

int wx = 200, wy = 200;

void CreateWindowFunc()
{
	window newwin = GUI::CreateWindow("New Window", wx, wy, 200, 100);
	if (!newwin) return;
	struct Label *lbl = (struct Label *)GUI::AddControl(newwin, CONTROL_LABEL);
	lbl->text = "Hello, World!";
	lbl->active = 1;
	lbl->visible = 1;
	wx += 5;
	wy += 5;
	if(wx > 400) wx = 0;
	if(wy > 400) wy = 0;
}

extern "C" void kmain()
{
	window win1 = GUI::CreateWindow("QuantumOS", 80, 60, 200, 100);

	struct TextBox *txt = (struct TextBox *)GUI::AddControl(win1, CONTROL_TEXTBOX);
	txt->active = 0;
	txt->visible = 1;

	struct Button *btn = (struct Button *)GUI::AddControl(win1, CONTROL_BUTTON);
	btn->label = "QOS Button";
	btn->w = 88;
	btn->y = 24;
	btn->visible = 1;
	btn->active = 1;
	btn->onClick = CreateWindowFunc;

	while (1) GUI::Update();
}
