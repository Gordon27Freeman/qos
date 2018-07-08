#include <gui.h>
#include <controls.h>
#include <string.h>
#include <mm.h>

int wx = 200, wy = 200;
struct TextBox *txt, *txt2;

void CreateWindowFunc()
{
	char *title = (char *)Memory::Alloc(1024);
	strcpy(title, txt2->text);
	window newwin = GUI::CreateWindow(title, wx, wy, 200, 100);
	if (!newwin) return;
	struct Label *lbl = (struct Label *)GUI::AddControl(newwin, CONTROL_LABEL);
	lbl->text = (char *)Memory::Alloc(1024);
	strcpy(lbl->text, txt->text);
	lbl->active = 1;
	lbl->visible = 1;
	wx += 5;
	wy += 5;
	if(wx > 400) wx = 200;
	if(wy > 400) wy = 200;
}

extern "C" void kmain()
{
	window win1 = GUI::CreateWindow("QuantumOS", 80, 60, 172, 74);

	struct Label *lbl = (struct Label *)GUI::AddControl(win1, CONTROL_LABEL);
	lbl->text = "Title:";
	lbl->active = 1;
	lbl->visible = 1;

	struct Label *lbl2 = (struct Label *)GUI::AddControl(win1, CONTROL_LABEL);
	lbl2->text = "Label:";
	lbl2->y = 24;
	lbl2->active = 1;
	lbl2->visible = 1;

	txt = (struct TextBox *)GUI::AddControl(win1, CONTROL_TEXTBOX);
	txt->active = 1;
	txt->visible = 1;
	txt->y = 24;
	txt->x = 54;
	txt->w = 112;

	txt2 = (struct TextBox *)GUI::AddControl(win1, CONTROL_TEXTBOX);
	txt2->active = 1;
	txt2->visible = 1;
	txt2->y = 0;
	txt2->x = 54;
	txt2->w = 112;

	struct Button *btn = (struct Button *)GUI::AddControl(win1, CONTROL_BUTTON);
	btn->label = "Create Window";
	btn->w = 166;
	btn->y = 48;
	btn->visible = 1;
	btn->active = 1;
	btn->onClick = CreateWindowFunc;

	while (1) GUI::Update();
}
